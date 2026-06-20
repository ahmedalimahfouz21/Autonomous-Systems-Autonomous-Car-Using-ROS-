#!/usr/bin/env python
# license removed for brevity
import rospy
from geometry_msgs.msg import Twist
from nav_msgs.msg import Odometry
import random
from tf.transformations import euler_from_quaternion, quaternion_from_euler

def Track(message):
    pos_message=message.pose.pose
    orient_message = message.pose.pose.orientation
    or_values = [orient_message.x, orient_message.y, orient_message.z, orient_message.w]
    (roll, pitch, yaw) = euler_from_quaternion (or_values)
    print("Position: " )
    print("x: " +str(pos_message.position.x))
    print("y: " +str(pos_message.position.y))
    print("z: " +str(pos_message.position.z))
    print("Orientation: " )
    print("roll : " + str(roll))
    print("pitch: " + str(pitch))
    print("yaw  : " + str(yaw))

def run():
  rospy.init_node('open_loop', anonymous=True)
  rospy.Subscriber("/husky_velocity_controller/odom", Odometry, Track)
  v_pub = rospy.Publisher('husky_velocity_controller/cmd_vel', Twist, queue_size=10)
  velo = Twist()
  velo.linear.y = 0
  velo.linear.z = 0
  velo.angular.x = 0
  velo.angular.y = 0
  rate = rospy.Rate(10) # 10hz
  while not rospy.is_shutdown():
    velo.linear.x = rospy.get_param('~x')
    velo.linear.y = rospy.get_param('~y')
    velo.angular.z = rospy.get_param('~z')
    v_pub.publish(velo)
    rate.sleep()
if __name__ == '__main__':
    try:
        run()
    except rospy.ROSInterruptException:
        pass


