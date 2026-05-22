import os
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import (
    DeclareLaunchArgument,
    IncludeLaunchDescription,
    ExecuteProcess,
    TimerAction,
)
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():

    # ── Chemins des packages ──────────────────────────────────────────────────
    pkg_elevator = os.path.join(
        os.path.expanduser('~'), 'ros2_ws', 'src', 'jackal_elevator'
    )
    pkg_multi = get_package_share_directory('multi_floor_nav')

    map_file   = os.path.join(pkg_multi, 'maps', 'floor0.yaml')
    map_file_0 = os.path.join(pkg_multi, 'maps', 'floor0.yaml')
    map_file_1 = os.path.join(pkg_multi, 'maps', 'floor1.yaml')
    rviz_config = os.path.join(pkg_elevator, 'rviz', 'nav_multi_floor.rviz')
    script_path = os.path.expanduser(
        '~/ros2_ws/src/jackal_elevator/scripts/robot_state_display.py'
    )

    params_robot1 = os.path.join(pkg_multi, 'configs', 'nav2_params_robot1.yaml')
    params_robot2 = os.path.join(pkg_multi, 'configs', 'nav2_params_robot2.yaml')

    use_sim_time = LaunchConfiguration('use_sim_time', default='true')
    gui          = LaunchConfiguration('gui',          default='true')
    front_laser  = LaunchConfiguration('front_laser',  default='true')

    # =========================================================================
    # Nœuds qui démarrent IMMÉDIATEMENT
    # =========================================================================
    immediate = [
        DeclareLaunchArgument('use_sim_time', default_value='true'),
        DeclareLaunchArgument('gui',          default_value='true'),
        DeclareLaunchArgument('front_laser',  default_value='true'),

        # ── 1. Gazebo + robot_state_publisher + spawn + bridge ────────────────
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(
                os.path.join(pkg_elevator, 'launch', 'jackal_repos_world.launch.py')
            ),
            launch_arguments={
                'gui':          gui,
                'use_sim_time': use_sim_time,
                'front_laser':  front_laser,
            }.items()
        ),

        # ── 2. Map servers partagés ───────────────────────────────────────────
        Node(
            package='nav2_map_server',
            executable='map_server',
            name='map_server',
            output='screen',
            parameters=[{'yaml_filename': map_file, 'use_sim_time': use_sim_time}]
        ),
        Node(
            package='nav2_map_server',
            executable='map_server',
            name='map_server_level0',
            output='screen',
            parameters=[{
                'use_sim_time': True,
                'yaml_filename': map_file_0,
                'topic_name':   'map_level_0',
                'frame_id':     'robot1/map',
            }],
        ),
        Node(
            package='nav2_map_server',
            executable='map_server',
            name='map_server_level1',
            output='screen',
            parameters=[{
                'use_sim_time': True,
                'yaml_filename': map_file_1,
                'topic_name':   'map_level_1',
                'frame_id':     'robot2/map',
            }],
        ),

        # ── 3a. AMCL — robot1 ────────────────────────────────────────────────
        Node(
            package='nav2_amcl',
            executable='amcl',
            name='amcl',
            namespace='robot1',
            output='screen',
            parameters=[params_robot1, {'use_sim_time': use_sim_time}],
            remappings=[
                ('scan', 'front/scan_fixed'),
                ('/tf', '/robot1/tf'),
                ('/tf_static', '/robot1/tf_static'),
                ('map', '/map'),
            ],
        ),

        # ── 3b. AMCL — robot2 ────────────────────────────────────────────────
        Node(
            package='nav2_amcl',
            executable='amcl',
            name='amcl',
            namespace='robot2',
            output='screen',
            parameters=[params_robot2, {'use_sim_time': use_sim_time}],
            remappings=[
                ('scan', 'front/scan_fixed'),
                ('/tf', '/robot2/tf'),
                ('/tf_static', '/robot2/tf_static'),
                ('map', '/map'),
            ],
        ),

        # ── 4. Lifecycle manager map servers ─────────────────────────────────
        Node(
            package='nav2_lifecycle_manager',
            executable='lifecycle_manager',
            name='lifecycle_manager_map_global',
            output='screen',
            parameters=[{
                'use_sim_time': use_sim_time,
                'autostart': True,
                'bond_timeout': 20.0,
                'node_names': [
                    'map_server',
                    'map_server_level0',
                    'map_server_level1',
                ],
            }]
        ),

        # ── 4b. Lifecycle manager AMCL robot1 ────────────────────────────────
        Node(
            package='nav2_lifecycle_manager',
            executable='lifecycle_manager',
            name='lifecycle_manager_map',
            namespace='robot1',
            output='screen',
            parameters=[{
                'use_sim_time': use_sim_time,
                'autostart': True,
                'bond_timeout': 20.0,
                'node_names': ['amcl'],
            }]
        ),

        # ── 4c. Lifecycle manager AMCL robot2 ────────────────────────────────
        Node(
            package='nav2_lifecycle_manager',
            executable='lifecycle_manager',
            name='lifecycle_manager_map',
            namespace='robot2',
            output='screen',
            parameters=[{
                'use_sim_time': use_sim_time,
                'autostart': True,
                'bond_timeout': 20.0,
                'node_names': ['amcl'],
            }]
        ),

        # ── TF relays ─────────────────────────────────────────────────────────
        Node(
    package='topic_tools',
    executable='relay',
    name='tf_relay_robot1',
    arguments=['/robot1/tf', '/tf'],
    parameters=[
        {'use_sim_time': True},
        {'input_qos_reliability': 'reliable'},
        {'output_qos_reliability': 'reliable'},
        {'input_qos_durability': 'volatile'},
        {'output_qos_durability': 'volatile'},
    ],
),
Node(
    package='topic_tools',
    executable='relay',
    name='tf_relay_robot2',
    arguments=['/robot2/tf', '/tf'],
    parameters=[
        {'use_sim_time': True},
        {'input_qos_reliability': 'reliable'},
        {'output_qos_reliability': 'reliable'},
        {'input_qos_durability': 'volatile'},
        {'output_qos_durability': 'volatile'},
    ],
),
        Node(
            package='topic_tools',
            executable='relay',
            name='map_relay_robot1',
            arguments=['/map', '/robot1/map'],
            parameters=[{'use_sim_time': True}],
        ),
        Node(
            package='topic_tools',
            executable='relay',
            name='map_relay_robot2',
            arguments=['/map', '/robot2/map'],
            parameters=[{'use_sim_time': True}],
        ),
        Node(
            package='topic_tools',
            executable='relay',
            name='tf_static_relay_to_robot1',
            arguments=['/tf_static', '/robot1/tf_static'],
            parameters=[{
                'use_sim_time': True,
                'input_qos_durability': 'transient_local',
                'output_qos_durability': 'transient_local',
            }],
        ),
        Node(
            package='topic_tools',
            executable='relay',
            name='tf_static_relay_to_robot2',
            arguments=['/tf_static', '/robot2/tf_static'],
            parameters=[{
                'use_sim_time': True,
                'input_qos_durability': 'transient_local',
                'output_qos_durability': 'transient_local',
            }],
        ),
        # ✅ OBLIGATOIRE — publier la frame world
        # frame map globale pour RViz
Node(
    package='tf2_ros',
    executable='static_transform_publisher',
    name='map_to_robot1_map',
    arguments=['0', '0', '0', '0', '0', '0', 'map', 'robot1/map']
),
Node(
    package='tf2_ros',
    executable='static_transform_publisher',
    name='map_to_robot2_map',
    arguments=['0', '0', '0', '0', '0', '0', 'map', 'robot2/map']
),

        # ── 7. RViz2 ─────────────────────────────────────────────────────────
        Node(
            package='rviz2',
            executable='rviz2',
            name='rviz2',
            output='screen',
            arguments=['-d', rviz_config],
            parameters=[{'use_sim_time': use_sim_time}],
        ),

        # ── 8. change_map_node ────────────────────────────────────────────────
        # launch file
        Node(
            package='multi_floor_nav',
            executable='change_map_node',
            namespace='robot1',       # ← crucial
            name='change_map_node',
        ),
        Node(
            package='multi_floor_nav',
            executable='change_map_node',
            namespace='robot2',       # ← crucial
            name='change_map_node',
        ),

        # ── 11. MQTT bridge ───────────────────────────────────────────────────
        Node(
            package='jackal_elevator',
            executable='elevator_mqtt_bridge.py',
            name='elevator_mqtt_bridge',
            output='screen',
            emulate_tty=True,
        ),

        # ── UI ────────────────────────────────────────────────────────────────
        ExecuteProcess(
            cmd=['python3', script_path],
            output='screen',
            name='robot_ui',
        ),
    ]
    
    # =========================================================================
    # Nav2 démarre après 15s — laisse AMCL publier map→odom avant les costmaps
    # =========================================================================
    delayed_nav2 = TimerAction(
        period=15.0,
        actions=[
            # ── 5a. Nav2 — robot1 ────────────────────────────────────────────
            Node(
                package='nav2_controller',
                executable='controller_server',
                name='controller_server',
                namespace='robot1',
                output='screen',
                parameters=[params_robot1, {'use_sim_time': use_sim_time}],
                remappings=[('/tf', 'tf'), ('/tf_static', 'tf_static')],
            ),
            Node(
                package='nav2_smoother',
                executable='smoother_server',
                name='smoother_server',
                namespace='robot1',
                output='screen',
                parameters=[params_robot1, {'use_sim_time': use_sim_time}],
                remappings=[('/tf', 'tf'), ('/tf_static', 'tf_static')],
            ),
            Node(
                package='nav2_planner',
                executable='planner_server',
                name='planner_server',
                namespace='robot1',
                output='screen',
                parameters=[params_robot1, {'use_sim_time': use_sim_time}],
                remappings=[('/tf', 'tf'), ('/tf_static', 'tf_static')],
            ),
            Node(
    package='nav2_behaviors',
    executable='behavior_server',
    name='behavior_server',
    namespace='robot1',
    output='screen',
    parameters=[
        params_robot1,
        {
            'use_sim_time': use_sim_time,
            'global_frame': 'robot1/odom',
            'robot_base_frame': 'robot1/base_link',
            'local_frame': 'robot1/odom',  # ← ajouter ça
        }
    ],
                remappings=[('/tf', 'tf'), ('/tf_static', 'tf_static')],
            ),
            Node(
                package='nav2_bt_navigator',
                executable='bt_navigator',
                name='bt_navigator',
                namespace='robot1',
                output='screen',
                parameters=[params_robot1, {'use_sim_time': use_sim_time}],
                remappings=[
                    ('navigate_to_pose', 'navigate_to_pose_internal'),
                    ('/tf', 'tf'),
                    ('/tf_static', 'tf_static'),
                ],
            ),
            Node(
                package='nav2_waypoint_follower',
                executable='waypoint_follower',
                name='waypoint_follower',
                namespace='robot1',
                output='screen',
                parameters=[params_robot1, {'use_sim_time': use_sim_time}],
            ),
            Node(
                package='nav2_velocity_smoother',
                executable='velocity_smoother',
                name='velocity_smoother',
                namespace='robot1',
                output='screen',
                parameters=[params_robot1, {'use_sim_time': use_sim_time}],
                remappings=[
                    ('cmd_vel',          'cmd_vel_nav'),
                    ('cmd_vel_smoothed', 'cmd_vel'),
                ],
            ),
            Node(
    package='nav2_collision_monitor',
    executable='collision_monitor',
    name='collision_monitor',
    namespace='robot1',
    output='screen',
    parameters=[params_robot1, {'use_sim_time': use_sim_time}],
    remappings=[('/tf', 'tf'), ('/tf_static', 'tf_static')],
),

            Node(
                package='nav2_lifecycle_manager',
                executable='lifecycle_manager',
                name='lifecycle_manager_navigation',
                namespace='robot1',
                output='screen',
                parameters=[{
                    'use_sim_time': True,
                    'autostart': True,
                    'bond_timeout': 20.0,
                    'node_names': [
                        
                        'controller_server',
                        'smoother_server',
                        'planner_server',
                        'behavior_server',
                        'bt_navigator',
                        'waypoint_follower',
                        'velocity_smoother',
                        'collision_monitor'
                    ],
                }]
            ),

            # ── 5b. Nav2 — robot2 ────────────────────────────────────────────
            Node(
                package='nav2_controller',
                executable='controller_server',
                name='controller_server',
                namespace='robot2',
                output='screen',
                parameters=[params_robot2, {'use_sim_time': use_sim_time}],
                remappings=[('/tf', 'tf'), ('/tf_static', 'tf_static')],
            ),
            Node(
                package='nav2_smoother',
                executable='smoother_server',
                name='smoother_server',
                namespace='robot2',
                output='screen',
                parameters=[params_robot2, {'use_sim_time': use_sim_time}],
                remappings=[('/tf', 'tf'), ('/tf_static', 'tf_static')],
            ),
            Node(
                package='nav2_planner',
                executable='planner_server',
                name='planner_server',
                namespace='robot2',
                output='screen',
                parameters=[params_robot2, {'use_sim_time': use_sim_time}],
                remappings=[('/tf', 'tf'), ('/tf_static', 'tf_static')],
            ),
            Node(
    package='nav2_behaviors',
    executable='behavior_server',
    name='behavior_server',
    namespace='robot2',
    output='screen',
    parameters=[
        params_robot1,
        {
            'use_sim_time': use_sim_time,
            'global_frame': 'robot2/odom',
            'robot_base_frame': 'robot2/base_link',
            'local_frame': 'robot2/odom',  # ← ajouter ça
        }
    ],
                remappings=[('/tf', 'tf'), ('/tf_static', 'tf_static')],
            ),
            Node(
                package='nav2_bt_navigator',
                executable='bt_navigator',
                name='bt_navigator',
                namespace='robot2',
                output='screen',
                parameters=[params_robot2, {'use_sim_time': use_sim_time}],
                remappings=[
                    ('navigate_to_pose', 'navigate_to_pose_internal'),
                    ('/tf', 'tf'),
                    ('/tf_static', 'tf_static'),
                ],
            ),
            Node(
                package='nav2_waypoint_follower',
                executable='waypoint_follower',
                name='waypoint_follower',
                namespace='robot2',
                output='screen',
                parameters=[params_robot2, {'use_sim_time': use_sim_time}],
            ),
            Node(
                package='nav2_velocity_smoother',
                executable='velocity_smoother',
                name='velocity_smoother',
                namespace='robot2',
                output='screen',
                parameters=[params_robot2, {'use_sim_time': use_sim_time}],
                remappings=[
                    ('cmd_vel',          'cmd_vel_nav'),
                    ('cmd_vel_smoothed', 'cmd_vel'),
                ],
            ),
            Node(
    package='nav2_collision_monitor',
    executable='collision_monitor',
    name='collision_monitor',
    namespace='robot2',
    output='screen',
    parameters=[params_robot2, {'use_sim_time': use_sim_time}],
    remappings=[('/tf', 'tf'), ('/tf_static', 'tf_static')],
),
            Node(
                package='nav2_lifecycle_manager',
                executable='lifecycle_manager',
                name='lifecycle_manager_navigation',
                namespace='robot2',
                output='screen',
                parameters=[{
                    'use_sim_time': True,
                    'autostart': True,
                    'bond_timeout': 20.0,
                    'node_names': [
                        
                        'controller_server',
                        'smoother_server',
                        'planner_server',
                        'behavior_server',
                        'bt_navigator',
                        'waypoint_follower',
                        'velocity_smoother',
                        'collision_monitor'
                    ],
                }]
            ),

            # ── 9. elevator_bt_executor ───────────────────────────────────────
            Node(
                package='multi_floor_nav',
                executable='elevator_bt_executor',
                name='elevator_bt_executor',
                namespace='robot1',
                output='screen',
                parameters=[{'use_sim_time': use_sim_time,
                             'robot_namespace': 'robot1',}],
            ),
            Node(
                package='multi_floor_nav',
                executable='elevator_bt_executor',
                name='elevator_bt_executor',
                namespace='robot2',
                output='screen',
                parameters=[{'use_sim_time': use_sim_time ,
                             'robot_namespace': 'robot2',}],
            ),

            # ── 10. goal_relay ────────────────────────────────────────────────
            ExecuteProcess(
                cmd=['python3',
                     os.path.join(pkg_elevator, 'scripts', 'goal_relay.py'),
                     '--namespace', 'robot1'],
                output='screen'
            ),
            ExecuteProcess(
                cmd=['python3',
                     os.path.join(pkg_elevator, 'scripts', 'goal_relay.py'),
                     '--namespace', 'robot2'],
                output='screen'
            ),
        ]
    )

    return LaunchDescription(immediate + [delayed_nav2])