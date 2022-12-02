import numpy
from scipy.special import expit as sigmoid


class neuralNetwork:

    def __init__(self, inputnodes, hiddennodes, outputnodes, learningrate) -> None:
        self.activation_function = lambda x: sigmoid(x)

        self.inodes = inputnodes
        self.hnodes = hiddennodes
        self.onodes = outputnodes

        self.lr = learningrate

        # 完全随机分布
        # self.wih=(numpy.random.rand(self.hnodes,self.inodes)-0.5)
        # self.who=(numpy.random.rand(self.onodes,self.hnodes)-0.5)

        # 正态分布
        self.wih = numpy.random.normal(
            0.0, pow(self.hnodes, -0.5), (self.hnodes, self.inodes))
        self.who = numpy.random.normal(
            0.0, pow(self.onodes, -0.5), (self.onodes, self.hnodes))

    def train(self, inputs_list, targets_list):
        inputs = numpy.array(inputs_list, ndmin=2).T
        targets = numpy.array(targets_list, ndmin=2).T

        hidden_inputs = numpy.dot(self.wih, inputs)
        hidden_outputs = self.activation_function(hidden_inputs)

        final_inputs = numpy.dot(self.who, hidden_outputs)
        final_outputs = self.activation_function(final_inputs)

        '''并不能复用
        # 复用，省事，防出错
        final_outputs = self.query(inputs_list)
        '''
        # error is the (target-actual)
        output_errors = targets-final_outputs
        # 按照权重分割再求和
        hidden_errors = numpy.dot(self.who.T, output_errors)

        # \Delta w_{j,k} = \alpha * E_k * sigmoid(O_k) * (1-sigmoid(O_k)) \cdot O_j^T
        self.who += self.lr * \
            numpy.dot((output_errors*final_outputs*(1.0-final_outputs)),
                      numpy.transpose(hidden_outputs))

        self.wih += self.lr * \
            numpy.dot((hidden_errors*hidden_outputs*(1.0-hidden_outputs)),
                      numpy.transpose(inputs))

    def query(self, inputs_list):
        inputs = numpy.array(inputs_list, ndmin=2).T

        hidden_inputs = numpy.dot(self.wih, inputs)
        hidden_outputs = self.activation_function(hidden_inputs)

        final_inputs = numpy.dot(self.who, hidden_outputs)
        final_outputs = self.activation_function(final_inputs)

        return final_outputs


'''
# For testing

n = neuralNetwork(inputnodes=3, hiddennodes=3, outputnodes=3, learningrate=0.3)

print(n.query([1.0, 0.5, -1.5]))
'''


def fit(x):
    if x == 0:
        return 0.01
    elif x == 1:
        return 0.50
    elif x == 2:
        return 0.99


def trans2list(strategy_num):
    try:
        strategy_num = int(strategy_num)
    except:
        print("Type error.")
        raise Exception

    east = fit(strategy_num % 3)
    strategy_num //= 3
    west = fit(strategy_num % 3)
    strategy_num //= 3
    south = fit(strategy_num % 3)
    strategy_num //= 3
    north = fit(strategy_num % 3)
    strategy_num //= 3
    middle = fit(strategy_num % 3)
    return [east, west, south, north, middle]


def trans4list(output_num):
    ret = [0.01, 0.01, 0.01, 0.01, 0.01]

    # 重新编码
    if output_num == 0:
        print('Bad strategy!')
        raise Exception
    if output_num == 5:
        output_num = 2
    elif output_num == 6:
        output_num = 5

    ret[output_num-1] = 0.99
    return ret


def trans2num(out_list):
    j = 4
    for i in range(4):
        if out_list[i] > out_list[j]:
            j = i
    if j == 4:
        return 6
    else:
        return j+1


def main():
    with open('strategy.txt', 'r', encoding='utf-8') as fobj:
        lines = [line for line in fobj.readlines() if line[0] != '#']

    # cnt = 0

    # 病句：
    # strategy = [int(i) for i in [line.split() for line in lines]]

    strategy = []

    for line in lines:
        for i in line.split():
            strategy.append(int(i))
            # cnt += 1

    # print(cnt)

    """ print("strategy:")
    for i in strategy:
        print(i) """

    ann = neuralNetwork(5, 5, 5, 0.08)

    for __ in range(4):
        for _ in range(100):
            for i in range(243):
                ann.train(trans2list(i), trans4list(strategy[i]))
        for i in range(243):
            print(trans2num(ann.query(trans2list(i))), end=' ')
        print(end='\n')


if __name__ == '__main__':
    main()
