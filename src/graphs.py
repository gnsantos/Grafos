from functools import reduce


def factorial(n):
    print(f'Calculating {n}!')
    return reduce(lambda x,y: x*y, range(1, n+1))

if __name__ == '__main__':
    print('Starting test')
    factorials = list(map(factorial, range(2,10)))
    print(f'Computed the following numbers: {factorials}')

