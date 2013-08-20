import random, math

def randInt(length):
    if length == 0:
        return 0
    sign = random.choice([-1, 1]);
    return random.randint(10 ** (length - 1), 10 ** length - 1) * sign

def multiplyTestGen(lengthRange):
    a = randInt(random.randint(0, lengthRange))
    b = randInt(random.randint(0, lengthRange))
    return (a, b, a * b)

def testWrap(test, operator):
    mask = []
    for i in range(3):
        if abs(test[i]) < 10 ** 18:
            mask.append(random.choice(['%sLL', '"%s"']))
        else:
            mask.append('"%s"')
             
    return (  'Assert::IsTrue(BigInteger(' + mask[0] + ') %s BigInteger(' + mask[1] + ') == BigInteger(' + mask[2] + '));'  ) % (test[0], operator, test[1], test[2])

def defaultMultiplyTestGen():
    
    file = open("tests\\multTests.cpp", 'w');

    for i in range(100):    
        file.write(testWrap(multiplyTestGen(2), '*'));
        file.write('\n');
    file.write('\n');    
    for i in range(100):    
        file.write(testWrap(multiplyTestGen(10), '*'));
        file.write('\n');
    file.write('\n');
    for i in range(1000):    
        file.write(testWrap(multiplyTestGen(1000), '*'));
        file.write('\n');
    file.write('\n');    
    for i in range(10):    
        file.write(testWrap(multiplyTestGen(10000), '*'));
        file.write('\n');
    

        
defaultMultiplyTestGen()