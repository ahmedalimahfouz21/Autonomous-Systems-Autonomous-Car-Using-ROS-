#!/usr/bin/env python
import nav_msgs.msg
import math
import time
import rospy
from geometry_msgs.msg import Twist
import sys, select, os
if os.name == 'nt':
  import msvcrt
else:
  import tty, termios

x=y=z=roll=pitch=yaw=0

def Track(data):
  global x,y,z,roll,pitch,yaw
  pose = data.pose.pose
  x= pose.position.x
  y = pose.position.y
  z = pose.position.z
  roll = pose.orientation.x
  pitch = pose.orientation.y
  yaw = pose.orientation.z


def getKey():
    if os.name == 'nt':
      if sys.version_info[0] >= 3:
        return msvcrt.getch().decode()
      else:
        return msvcrt.getch()

    tty.setraw(sys.stdin.fileno())
    rlist, _, _ = select.select([sys.stdin], [], [], 0.1)
    if rlist:
        key = sys.stdin.read(1)
    else:
        key = ''

    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
    return key

def print_pos():
    print('Position')
    print('x:')
    print(x)
    print('y:')
    print(y)
    print('z:')
    print(z)
    print('Orientaion')
    print('roll:')
    print(roll)
    print('pitch:')
    print(pitch)
    print('yaw:')
    print(yaw)


def move(v_pub):
    velo = Twist()
    velo.linear.x = 0
    velo.linear.y = 0 
    velo.linear.z = 0
    velo.angular.x = 0.0 
    velo.angular.y = 0.0 
    velo.angular.z = 0.0
    while(1):
          print_pos()
          key = getKey()
          if key == 'y' :
                   velo.linear.x = velo.linear.x + 1
          elif key=='h':
                   velo.linear.x = velo.linear.x - 1
          elif key=='j':
                   velo.angular.z = velo.angular.z + 1 
          elif key=='g':
                   velo.angular.z = velo.angular.z - 1 
          else:
             if (key == '\x03'):
                  break 
          v_pub.publish(velo)  

          velo.linear.x = 0.0
          velo.linear.y = 0.0 
          velo.linear.z = 0.0
          velo.angular.x = 0.0 
          velo.angular.y = 0.0 
          velo.angular.z = 0.0   
    
def run():
    rospy.init_node('Teleport')
    v_pub = rospy.Publisher('cmd_vel', Twist, queue_size=10)
    rospy.Subscriber('/husky_velocity_controller/odom', nav_msgs.msg.Odometry, Track)
    try: 
      move(v_pub) 
    except rospy.ROSInterruptException:
        pass               



if __name__=="__main__":
  
    if os.name != 'nt':
        settings = termios.tcgetattr(sys.stdin)
    run()
          
    
