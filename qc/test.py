import time
import subprocess
import cereal.messaging as messaging
import argparse
import psutil

sm = messaging.SubMaster(['deviceState'], poll='deviceState')

# mount /dev/sdg1 /data/external/
EXTERNAL_DEV = "/dev/sdg1"
MOUNT_POINT = "/data/external"
BACKUP_DIR = "/data/external/comma3x"

EXTERNAL_DEV = "/dev/sdg4"
MOUNT_POINT = "/data/external"
BACKUP_DIR = "/data/external/home/heqichen/workspace/comma/data/"

def is_external_mounted(target_device, target_mount_point):
    output = subprocess.check_output(['mount'])
    mounts = output.decode('utf-8').split('\n')
    for line in mounts:
        if not line.strip():
            continue  # 跳过空行
        parts = line.split()
        if len(parts) >= 2:
            device = parts[0]
            mount_point = parts[2]  # 第二个元素是设备，第三个是挂载点
            if (target_device == device) and (mount_point == target_mount_point):
               return True
    return False

def mount_external(device, mount_point):
   return subprocess.call(['sudo', 'mount', device, mount_point])

def unmount_external(mount_point):
   return subprocess.call(['sudo', 'umount', mount_point])

def sync_data():
   return subprocess.call(['rsync', '-aP', "/data/media", BACKUP_DIR])

def check_disk_usage():
  psutil.disk_usage(MOUNT_POINT)
  #sdiskusage(total=244939624448, used=152461508608, free=79961317376, percent=65.6)  

def onroad_routine():
  print("onroad")
  # target: rsync all data to disk
  if not is_external_mounted(EXTERNAL_DEV, MOUNT_POINT):
    mount_external(EXTERNAL_DEV, MOUNT_POINT)
  # check if mounted
  if not is_external_mounted(EXTERNAL_DEV, MOUNT_POINT):
     return
  # all good, let's sync
  sync_data()

  # check disk usage
  check_disk_usage()
    

def offroad_routine():
  print("offroad")
  if is_external_mounted(EXTERNAL_DEV, MOUNT_POINT):
     unmount_external(MOUNT_POINT)

def routine(force_onroad=False):
    sm.update(0)
    onroad = sm.all_checks(['deviceState']) and sm['deviceState'].started
    if (force_onroad or onroad):
        onroad_routine()
    else:
        offroad_routine()


def main(force_onroad=False):
  # run routine forever
  routine(force_onroad)


if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument('--force-onroad', action='store_true', help='force onroad status')
  args = parser.parse_args()

  main(args.force_onroad)


