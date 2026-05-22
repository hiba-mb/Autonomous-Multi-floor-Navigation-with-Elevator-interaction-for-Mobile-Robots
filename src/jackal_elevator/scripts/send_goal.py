#!/usr/bin/env python3
"""
send_goal.py — Envoi d'un goal Nav2 avec timeout explicite
Compatible avec le BT executor qui intercepte navigate_to_pose.

Usage:
  python3 send_goal.py [--namespace robot1|robot2] <floor> <x> <y> [yaw_rad]

Retourne:
  exit 0 + print "Mission accomplie !" si status==4 (SUCCEEDED)
  exit 1 sinon
"""

import sys
import math
import time
import rclpy
from rclpy.node import Node
from rclpy.action import ActionClient
from rclpy.executors import MultiThreadedExecutor
from nav2_msgs.action import NavigateToPose
from std_msgs.msg import Int32
from action_msgs.msg import GoalStatus


def main():
    # ── Parser les arguments ──────────────────────────────────────────────────
    namespace = "robot1"
    args = sys.argv[1:]

    if "--namespace" in args:
        idx = args.index("--namespace")
        if idx + 1 < len(args):
            namespace = args[idx + 1]
            args.pop(idx)
            args.pop(idx)

    if len(args) < 3:
        print("Usage: python3 send_goal.py [--namespace robot1|robot2] <floor> <x> <y> [yaw]")
        print("Ex:    python3 send_goal.py --namespace robot1 1 7.3 -5.5 3.14")
        sys.exit(1)

    floor = int(args[0])
    x     = float(args[1])
    y     = float(args[2])
    yaw   = float(args[3]) if len(args) > 3 else 0.0

    print(f"[send_goal] namespace={namespace} floor={floor} x={x} y={y} yaw={yaw:.4f}rad")

    # ── Init ROS2 ─────────────────────────────────────────────────────────────
    rclpy.init()
    node = Node("goal_sender")

    # MultiThreadedExecutor : les callbacks result/feedback arrivent
    # même pendant spin_until_future_complete
    executor = MultiThreadedExecutor()
    executor.add_node(node)

    action_name = f"/{namespace}/navigate_to_pose"
    floor_topic = f"/{namespace}/goal_floor"

    # ── Publisher étage ───────────────────────────────────────────────────────
    floor_pub = node.create_publisher(Int32, floor_topic, 10)

    # ── Action client ─────────────────────────────────────────────────────────
    action_client = ActionClient(node, NavigateToPose, action_name)

    print(f"[send_goal] Attente action server {action_name}...")
    if not action_client.wait_for_server(timeout_sec=10.0):
        print(f"[send_goal] Action server {action_name} non disponible !")
        node.destroy_node()
        rclpy.shutdown()
        sys.exit(1)

    print(f"[send_goal] Action server prêt ✓")

    # ── Publier l'étage cible ─────────────────────────────────────────────────
    # Attendre un subscriber (BT executor lit /goal_floor)
    timeout_count = 0
    while floor_pub.get_subscription_count() == 0 and timeout_count < 30:
        executor.spin_once(timeout_sec=0.1)
        timeout_count += 1

    floor_msg      = Int32()
    floor_msg.data = floor
    floor_pub.publish(floor_msg)
    print(f"[send_goal] Étage {floor} publié sur {floor_topic}")

    # ── Construire le goal ────────────────────────────────────────────────────
    goal_msg = NavigateToPose.Goal()
    goal_msg.pose.header.frame_id = f"{namespace}/map"   # ✅ correct
    goal_msg.pose.header.stamp        = node.get_clock().now().to_msg()
    goal_msg.pose.pose.position.x     = x
    goal_msg.pose.pose.position.y     = y
    goal_msg.pose.pose.orientation.z  = math.sin(yaw / 2.0)
    goal_msg.pose.pose.orientation.w  = math.cos(yaw / 2.0)

    # ── Variables de résultat ─────────────────────────────────────────────────
    goal_done      = False
    goal_succeeded = False
    goal_status    = -1
    goal_handle    = None

    def result_cb(future):
        nonlocal goal_done, goal_succeeded, goal_status
        result = future.result()
        goal_status    = result.status
        goal_succeeded = (result.status == GoalStatus.STATUS_SUCCEEDED)
        goal_done      = True
        print(f"[send_goal] Résultat reçu — status={result.status} "
              f"({'SUCCEEDED' if goal_succeeded else 'FAILED'})")

    def goal_response_cb(future):
        nonlocal goal_handle, goal_done
        gh = future.result()
        if not gh.accepted:
            print("[send_goal] Goal rejeté — BT déjà en cours ?")
            goal_done = True
            return
        goal_handle = gh
        print("[send_goal] Goal accepté ✓ — en attente du résultat...")
        result_future = gh.get_result_async()
        result_future.add_done_callback(result_cb)

    # ── Envoyer le goal ───────────────────────────────────────────────────────
    print(f"[send_goal] Envoi goal → {action_name} ({x:.3f}, {y:.3f})")
    send_future = action_client.send_goal_async(goal_msg)
    send_future.add_done_callback(goal_response_cb)

    # ── Attendre le résultat avec timeout explicite ───────────────────────────
    # FIX PRINCIPAL : ne pas utiliser spin_until_future_complete qui bloque
    # indéfiniment. Utiliser une boucle avec timeout pour détecter la navigation
    # terminée même si le result_callback tarde.
    GOAL_TIMEOUT  = 300.0   # secondes (< 180s watchdog FM)
    POLL_INTERVAL = 0.1     # secondes
    start_time    = time.time()

    while not goal_done:
        elapsed = time.time() - start_time

        # Timeout de sécurité
        if elapsed > GOAL_TIMEOUT:
            print(f"[send_goal] TIMEOUT local après {elapsed:.0f}s — abandon")
            # Annuler le goal en cours
            if goal_handle is not None:
                try:
                    cancel_future = goal_handle.cancel_goal_async()
                    executor.spin_until_future_complete(
                        cancel_future, timeout_sec=2.0)
                    print("[send_goal] Goal annulé")
                except Exception as e:
                    print(f"[send_goal] Erreur annulation: {e}")
            break

        # Spin pour recevoir les callbacks
        executor.spin_once(timeout_sec=POLL_INTERVAL)

        # Log de progression toutes les 10s
        if int(elapsed) % 10 == 0 and int(elapsed) > 0:
            status_str = "en attente goal_handle" if goal_handle is None else "EXECUTING"
            print(f"[send_goal] En cours... {elapsed:.0f}s ({status_str})")

    # ── Résultat final ────────────────────────────────────────────────────────
    elapsed_total = time.time() - start_time

    if goal_succeeded:
        print(f"[send_goal] Mission accomplie ! ({elapsed_total:.1f}s)")
        exit_code = 0
    else:
        print(f"[send_goal] Mission echouee (status={goal_status}, {elapsed_total:.1f}s)")
        exit_code = 1

    # ── Cleanup ───────────────────────────────────────────────────────────────
    node.destroy_node()
    executor.shutdown()
    rclpy.shutdown()
    sys.exit(exit_code)


if __name__ == "__main__":
    main()