import cereal.messaging as messaging
from openpilot.common.realtime import Ratekeeper


class Test:
  def __init__(self):
    self.sm = messaging.SubMaster(['qcPilotCufuState'])
    self.rk = Ratekeeper(100, print_delay_threshold=None)

  def run(self):
    while True:
      self.rk.keep_time()
      self.step()

  def step(self):
    self.sm.update(0)
    print(self.sm['qcPilotCufuState'])


def main():
  t = Test()
  t.run()


if __name__ == "__main__":
  main()
