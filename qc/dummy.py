from openpilot.common.params import Params
import time


params = Params()

if __name__ == "__main__":
  while True:
    print(params.get_bool("IsOnroad"))
    time.sleep(1.0)
