import cereal.messaging as messaging

NAME_MAPPING = {
  0x9D: "CRZ_BTNS",
  0x243: "CAM_LKAS",
  0x440: "CAM_LANEINFO"
}

if __name__ == "__main__":
  sm = messaging.sub_sock('peripheralState') # pandaStates
  start_time = -1
  while True:
    sm.setTimeout(1000)
    msg = messaging.recv_sock(sm, True)
    if msg is not None:
      print(msg)


      #can_msgs = msg.sendcan
      #for can_msg in can_msgs:



