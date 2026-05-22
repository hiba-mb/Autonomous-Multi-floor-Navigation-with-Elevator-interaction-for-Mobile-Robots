import os
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, ExecuteProcess
from launch.substitutions import LaunchConfiguration, Command
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    pkg_elevator = os.path.join(os.path.expanduser('~'), 'ros2_ws', 'src', 'jackal_elevator')
    jackal_xacro = os.path.join(pkg_elevator, 'urdf', 'jackal.urdf.xacro')
    world_file   = os.path.join(pkg_elevator, 'worlds', 'modified_elevator.world')
    pkg_multi = get_package_share_directory('multi_floor_nav')

    # ── Positions robot1 ─────────────────────────────────────────────────────
    x_pos  = LaunchConfiguration('x_pos',  default='7.3')
    y_pos  = LaunchConfiguration('y_pos',  default='-5.5')
    z_pos  = LaunchConfiguration('z_pos',  default='0.2')
    yaw    = LaunchConfiguration('yaw',    default='3.14')

    # ── Positions robot2 ─────────────────────────────────────────────────────
    x_pos2 = LaunchConfiguration('x_pos2', default='7.6')
    y_pos2 = LaunchConfiguration('y_pos2', default='1.54')
    z_pos2 = LaunchConfiguration('z_pos2', default='0.2')
    yaw2   = LaunchConfiguration('yaw2',   default='3.14')

    gui = LaunchConfiguration('gui', default='true')

    return LaunchDescription([
        DeclareLaunchArgument('use_sim_time', default_value='true'),
        DeclareLaunchArgument('gui',          default_value='true'),
        DeclareLaunchArgument('x_pos',        default_value='7.3'),
        DeclareLaunchArgument('y_pos',        default_value='-5.5'),
        DeclareLaunchArgument('z_pos',        default_value='0.2'),
        DeclareLaunchArgument('yaw',          default_value='3.14'),
        DeclareLaunchArgument('x_pos2',       default_value='7.6'),
        DeclareLaunchArgument('y_pos2',       default_value='1.56'),
        DeclareLaunchArgument('z_pos2',       default_value='0.2'),
        DeclareLaunchArgument('yaw2',         default_value='3.14'),
        DeclareLaunchArgument('front_laser',  default_value='true'),

        # ── 1. Gazebo ─────────────────────────────────────────────────────────
        ExecuteProcess(
            cmd=['gz', 'sim', '-r', '--headless-rendering', world_file],
            additional_env={
                '__GL_SYNC_TO_VBLANK': '0',
                'LIBGL_ALWAYS_SOFTWARE': '0',
            },
            output='screen'
        ),

        # ── 2a. Robot State Publisher ── robot1 ───────────────────────────────
        # IMPORTANT : frame_prefix = 'robot1/' pour que les joints du URDF
        # (base_link, front_laser, etc.) soient publiés sous robot1/base_link etc.
        # Cela doit correspondre exactement aux frames que Gazebo publie via le bridge.
        # robot1
Node(
    package='robot_state_publisher',
    executable='robot_state_publisher',
    name='robot_state_publisher',
    namespace='robot1',
    parameters=[{
        'use_sim_time': True,
        'robot_description': ParameterValue(
            Command(['xacro ', jackal_xacro, ' robot_ns:=robot1']),
            value_type=str
        ),
        'frame_prefix': 'robot1/',
    }]
),

# robot2
Node(
    package='robot_state_publisher',
    executable='robot_state_publisher',
    name='robot_state_publisher',
    namespace='robot2',
    parameters=[{
        'use_sim_time': True,
        'robot_description': ParameterValue(
            Command(['xacro ', jackal_xacro, ' robot_ns:=robot2']),
            value_type=str
        ),
        'frame_prefix': 'robot2/',
    }]
),

        # ── 3a. Spawn ── robot1 ───────────────────────────────────────────────
        Node(
            package='ros_gz_sim',
            executable='create',
            name='spawn_jackal',
            arguments=[
                '-name',  'jackal',
                '-topic', '/robot1/robot_description',
                '-x', x_pos, '-y', y_pos, '-z', z_pos, '-Y', yaw,
            ],
            output='screen'
        ),

        # ── 3b. Spawn ── robot2 ───────────────────────────────────────────────
        Node(
            package='ros_gz_sim',
            executable='create',
            name='spawn_jackal2',
            arguments=[
                '-name',  'jackal2',
                '-topic', '/robot2/robot_description',
                '-x', x_pos2, '-y', y_pos2, '-z', z_pos2, '-Y', yaw2,
            ],
            output='screen'
        ),

        # ── 4. Bridge ─────────────────────────────────────────────────────────
        #
        # CORRECTION TF :
        # Gazebo publie les TF du modèle 'jackal'  sur /model/jackal/pose
        # et du modèle 'jackal2' sur /model/jackal2/pose.
        # On bridge ces poses séparément vers /robot1/tf et /robot2/tf,
        # puis on les remet sur /tf global SANS les mélanger.
        #
        # Le bridge TF Gazebo→ROS publie les frames avec le nom du modèle SDF.
        # Si le modèle s'appelle 'jackal', les frames seront 'jackal/base_link' etc.
        # Le robot_state_publisher avec frame_prefix='robot1/' publie 'robot1/base_link'.
        # → Il faut que les noms correspondent : soit on retire frame_prefix et on
        #   utilise les noms Gazebo, soit on garde frame_prefix et on n'utilise PAS
        #   le bridge TF de Gazebo (on laisse robot_state_publisher + EKF gérer TF).
        #
        # CHOIX : on garde frame_prefix='robot1/' et on NE bridge PAS les TF Gazebo
        # (on commente les lignes /robotX/tf). Le robot_state_publisher publie les
        # transforms joints, l'EKF publie odom→base_link, AMCL publie map→odom.
        #
        Node(
    package='ros_gz_bridge',
    executable='parameter_bridge',
    name='gz_bridge',
    arguments=[
        '/clock@rosgraph_msgs/msg/Clock[gz.msgs.Clock',

        # robot1
        '/robot1/cmd_vel@geometry_msgs/msg/Twist]gz.msgs.Twist',
        '/robot1/odometry/filtered@nav_msgs/msg/Odometry[gz.msgs.Odometry',        '/robot1/front/scan@sensor_msgs/msg/LaserScan[gz.msgs.LaserScan',
        '/robot1/imu/data@sensor_msgs/msg/Imu[gz.msgs.IMU',
        '/elevator@std_msgs/msg/String]gz.msgs.StringMsg',
        '/elevator/door_joint/cmd@std_msgs/msg/Float64]gz.msgs.Double',

        # robot2
        '/robot2/cmd_vel@geometry_msgs/msg/Twist]gz.msgs.Twist',
        '/robot2/odometry/filtered@nav_msgs/msg/Odometry[gz.msgs.Odometry',        '/robot2/front/scan@sensor_msgs/msg/LaserScan[gz.msgs.LaserScan',
        '/robot2/imu/data@sensor_msgs/msg/Imu[gz.msgs.IMU',
        

        
    ],
    
    parameters=[{'use_sim_time': True, 'expand_gz_topic_names': True}],
    output='screen'
),

Node(
    package='ros_gz_bridge',
    executable='parameter_bridge',
    name='elevator_joint_state_bridge',
    arguments=[
        '/world/default/model/elevator/joint_state@sensor_msgs/msg/JointState[gz.msgs.Model',
    ],
    parameters=[{'use_sim_time': True}],
    output='screen'
),

        # ── 5a. EKF ── robot1
Node(
    package='robot_localization',
    executable='ekf_node',
    name='ekf_filter_node',
    namespace='robot1',
    output='screen',
    parameters=[
        os.path.join(pkg_multi, 'configs', 'ekf_robot1.yaml'),
        {'use_sim_time': True},
    ],
    remappings=[
        ('odometry/filtered', 'odometry/filtered'),
        ('odometry/raw',      'odometry/raw'),
        ('imu/data',          'imu/data'),
        ('/tf',               '/robot1/tf'),        # ← AJOUTE
        ('/tf_static',        '/robot1/tf_static'),
    ]
),

# ── 5b. EKF ── robot2
Node(
    package='robot_localization',
    executable='ekf_node',
    name='ekf_filter_node',
    namespace='robot2',
    output='screen',
    parameters=[
        os.path.join(pkg_multi, 'configs', 'ekf_robot2.yaml'),
        {'use_sim_time': True},
    ],
    remappings=[
        ('odometry/filtered', 'odometry/filtered'),
        ('odometry/raw',      'odometry/raw'),
        ('imu/data',          'imu/data'),
        ('/tf',               '/robot2/tf'),        # ← AJOUTE
        ('/tf_static',        '/robot2/tf_static'),
            # ← AJOUTE
    ]
),
        # ── 6a. scan_reframer ── robot1 ───────────────────────────────────────
        Node(
            package='multi_floor_nav',
            executable='scan_reframer_node.py',
            name='scan_reframer',
            namespace='robot1',
            output='screen',
            parameters=[{'use_sim_time': True}],
        ),

        # ── 6b. scan_reframer ── robot2 ───────────────────────────────────────
        Node(
            package='multi_floor_nav',
            executable='scan_reframer_node.py',
            name='scan_reframer',
            namespace='robot2',
            output='screen',
            parameters=[{'use_sim_time': True}],
        ),
        Node(
    package='ros_gz_bridge',
    executable='parameter_bridge',
    name='joint_state_bridge_robot1',
    arguments=[
        '/robot1/joint_states@sensor_msgs/msg/JointState[gz.msgs.Model',
    ],
    parameters=[{
        'use_sim_time': True,
        'reliability': 'best_effort',  # ← ajouter
    }],
    output='screen'
),

Node(
    package='ros_gz_bridge',
    executable='parameter_bridge',
    name='joint_state_bridge_robot2',
    arguments=[
        '/robot2/joint_states@sensor_msgs/msg/JointState[gz.msgs.Model',
    ],
    parameters=[{
        'use_sim_time': True,
        'reliability': 'best_effort',  # ← ajouter
    }],
    output='screen'
),

        # ── 7a. odom_covariance_fixer ── robot1 ───────────────────────────────
       # ── 6a. scan_reframer — already correct in your launch, keep as Node ✅

# ── 7a. odom_covariance_fixer — robot1 (change from ExecuteProcess to Node)
Node(
    package='multi_floor_nav',
    executable='odom_covariance_fixer.py',
    name='odom_covariance_fixer',
    namespace='robot1',
    output='screen',
    parameters=[{'use_sim_time': True}],
),

# ── 7b. odom_covariance_fixer — robot2
Node(
    package='multi_floor_nav',
    executable='odom_covariance_fixer.py',
    name='odom_covariance_fixer',
    namespace='robot2',
    output='screen',
    parameters=[{'use_sim_time': True}],
),
    ])