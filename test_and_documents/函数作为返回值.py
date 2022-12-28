def foo(num):
    # 该函数返回一个函数
    return lambda x: x*num

print( (foo(10)) (9) ) # 输出90