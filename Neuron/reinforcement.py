import logging
import numpy
from random import randint
from gym import spaces
import gym

logger = logging.getLogger(__name__)


class GridEnv(gym.Env):
    metadata = {
        'render.modes': ['human', 'rgb_array'],
        'video.frames_per_second': 2
    }

    def __init__(self) -> None:
        # 状态空间，格子按照index = i * 10 + j 编号
        self.states = list(range(100))

        # 动作集合：上下左右过捡（去掉了随机）
        self.actions = [1, 2, 3, 4, 5, 6]

        # 回报函数
        self.rewards = dict()
        for i in range(10):  # 撞墙扣分
            self.rewards[(i, 1)] = -5
            self.rewards[(i+90, 2)] = -5
            self.rewards[(i*10, 3)] = -5
            self.rewards[(i*10+9, 4)] = -5
        self.rewards[(-1, 6)] = 10  # 合法捡罐子
        self.rewards[(-2, 6)] = -2  # 非法捡罐子

        # 随机生成棋盘
        self.grid = []
        self.count = 0
        for i in range(100):
            x = randint(0, 1)
            self.count += x
            self.grid.append(x)

        # 终止状态
        self.terminate_states = {0: 1}

        # 状态转移
        self.t = dict()

        for i in range(10):  # 这里暂且不考虑碰壁
            for j in range(10):
                self.t[(i*10+j, 1)] = (i-1)*10+j
                self.t[(i*10+j, 2)] = (i+1)*10+j
                self.t[(i*10+j, 3)] = i*10+j-1
                self.t[(i*10+j, 4)] = i*10+j+1

        for i in range(10):  # 指正这些碰壁的
            self.t[(i, 1)] = i
            self.t[(i+90, 2)] = i+90
            self.t[(i*10, 3)] = i*10
            self.t[(i*10+9, 4)] = i*10+9

        # 折扣因子，这里选0.998是因为0.998**200得0.670
        self.gamma = 0.998
        # 留空
        self.viewer = None
        # 起始状态左上角
        self.state = 0

    # 一簇平凡的成员函数:
    def getTerminal(self):
        return self.terminate_states

    def getGamma(self):
        return self.gamma

    def getStates(self):
        return self.states

    def getAction(self):
        return self.actions

    def getTerminate_states(self):
        return self.terminate_states

    def setAction(self, s):
        self.state = s

    # 行动
    def _step(self, action):
        # 退出条件
        if self.count == 0:
            return self.state, 0, True, {}

        # 状态转移
        if action == 6:  # 捡罐子
            exi = self.grid[self.state]-2  # 有则-1，无则-2
            if exi == -1:  # 如果有罐子
                self.grid[self.state] = 0
                self.count-1
            key = (exi, 6)
        else:
            key = (self.state, action)
            if key in self.t:
                self.state = self.t[key]
            else:  # 若action不是1234，则不引起状态变化
                pass

        if key not in self.rewards:
            r = 0.0
        else:
            r = self.rewards[key]

        return self.state, r, False, {}

    def _reset(self):
        self.__init__()  # 重新生成数据吧。
        return self.state

    def render(self, mode='human', close=False):
        if close:
            if self.viewer is not None:
                self.viewer.close()
                self.viewer = None
            return
        pass  # GUI部分用C++写了，此处不再重复


def main():
    env = gym.make('YellowChan')
    env.reset()
    env.render()


if __name__ == '__main__':
    main()
