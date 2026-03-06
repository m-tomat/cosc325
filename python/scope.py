x=2
def outer():
    global x # this pulls in the global x
    x=3
    def inner():
        x=5 # this creates a NEW local x
        def reallyinner():
            nonlocal x # this pulls in the local x from inner
            x=x+1
            print(x) 
        reallyinner()
        print(x)
    inner()
    print(x)

def addx(x, n):
    return x+n

def make_adder(n):
    return lambda x:x+n # anonymous function being created and returned
                        # the function body is return x+n

if __name__ == '__main__':
    outer()
    print(x)
    add5 = make_adder(5)
    add10 = make_adder(10)
    print(add5(3), add10(3))
    print(addx(3,5), addx(3,10))
    n=2
    print((lambda x:x+n)(n))
