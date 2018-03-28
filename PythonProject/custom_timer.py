import time


class Timer():
    TOTAL_TIMERS = 0

    def __init__(self, name=None, report=True):
        if name is not None:
            self.name = name
        else:
            name = "Timer_" + str(Timer.TOTAL_TIMERS)

        self.start_time = 0
        self.report = report
        Timer.TOTAL_TIMERS += 1

    def __enter__(self):
        self.start_time = time.time()
        return self
        
    def __exit__(self, type, value, traceback):
        #Exception handling here
        self.end_time = time.time()
        elapsed = round(self.end_time - self.start_time, 3)

        if self.report:
            print(self.name + ' time taken = ' + str(elapsed))


    def so_far(self):
        cur_time = time.time()
        elapsed = cur_time - self.start_time
        return elapsed


if __name__ == '__main__':

    with Timer('test') as timer:
        for i in range(10000):
            print(timer.so_far())
    