import cereal.messaging as messaging

NAME_MAPPING = {
  0x9D: "CRZ_BTNS",
  0x243: "CAM_LKAS",
  0x440: "CAM_LANEINFO"
}

if __name__ == "__main__":
  sm = messaging.sub_sock('sendcan')
  start_time = -1
  while True:
    sm.setTimeout(1000)
    msg = messaging.recv_sock(sm, True)
    if msg is not None:
      log_time = msg.logMonoTime
      timestamp = 0

      if start_time >= 0:
        timestamp = log_time - start_time
      else:
        start_time = log_time
        timestamp = 0


      for can_msg in msg.sendcan:
        can_id = "{}".format(hex(can_msg.address))

        if can_msg.address in NAME_MAPPING.keys():
          can_id = NAME_MAPPING[can_msg.address]

        print("[{} {}:{}] ".format(float(timestamp) / 1000000000.0, can_msg.src, can_id))
        # print(msg)


      #can_msgs = msg.sendcan
      #for can_msg in can_msgs:



