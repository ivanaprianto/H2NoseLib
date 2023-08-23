import math

def calculateLin(rl, ppm, m, b, r0):
    ppm_log = math.log10(ppm)
    c = ppm_log*m+b
    ratio = 10**c
    rs = ratio*r0
    volt = (5.0 * rl) / (rs + rl)
    readout = (volt/5.0)*1023
    print(readout)
    print(volt)

def calculateLog(rl, ppm, m, b, r0):
    ratio = (ppm / m)**(1.0/b)
    rs = ratio*r0
    volt = (5.0 * rl) / (rs + rl)
    readout = (volt/5.0)*1023
    print(readout)
    print(volt)

def calculateRL(ppm, readout, m, b, r0):
    volt = readout*(5.0/1023)
    ppm_log = math.log10(ppm)
    c = ppm_log*m+b
    ratio = 10**c
    rs = ratio*r0
    rl = (volt*rs) / (5.0-volt)
    print(rl)

def calculateB(ppm, readout, m, r0, rl):
    volt = readout*(5.0/1023)
    rs = ((5.0*rl)/volt)-rl
    ratio = rs/r0
    ratio_log = math.log10(ratio)
    ppm_log = math.log10(ppm)
    b = ratio_log - (m*ppm_log)
    print(b)

def calculateM(ppm, readout, b, r0, rl):
    volt = readout*(5.0/1023)
    rs = ((5.0*rl)/volt)-rl
    ratio = rs/r0
    ratio_log = math.log10(ratio)
    ppm_log = math.log10(ppm)
    m = (ratio_log - b) / ppm_log
    print(m)

def calculatePPMLog(readout, m, b, r0, rl):
    volt = readout*(5.0/1023)
    rs = ((5.0*rl)/volt)-rl
    ratio = rs/r0
    ppm = m*(ratio**b)
    print(ppm)

def calculatePPMLin(readout, m, b, r0, rl):
    volt = readout*(5.0/1023)
    rs = ((5.0*rl)/volt)-rl
    ratio = rs/r0
    ppm_log = (math.log10(ratio)-b)/m
    ppm = 10**ppm_log
    print(ppm)

def calculateBoth(readout1, readout2, readout3, r0, rl, ppm1, ppm2, ppm3):
    ratio1 = ((5.0*rl)/(readout1*(5.0/1023))-rl)/r0
    ratio2 = ((5.0*rl)/(readout2*(5.0/1023))-rl)/r0
    ratio3 = ((5.0*rl)/(readout3*(5.0/1023))-rl)/r0
    ratioSum = ratio1+ratio2+ratio3
    ratioLogSum = math.log10(ratio1) + math.log10(ratio2) + math.log10(ratio3)

    ppm1_log = math.log10(ppm1)
    ppm2_log = math.log10(ppm2)
    ppm3_log = math.log10(ppm3)
    ppm_sum = ppm1_log + ppm2_log + ppm3_log
    ppmsq_sum = (ppm1_log**2) + (ppm2_log**2) + (ppm3_log**2)

    ratioPPMSum = (ppm1_log*math.log10(ratio1))+(ppm2_log*math.log10(ratio2))+(ppm3_log*math.log10(ratio3))

    meanPPM = ppm_sum / 3
    meanRatio = ratioLogSum / 3

    m = (ratioPPMSum - 3 * meanPPM * meanRatio) / (ppmsq_sum - 3 * meanPPM * meanPPM)
    b = meanRatio - m * meanPPM
    print("b : ", b)
    print("m : ", m)

def calculateBothTwoData(readout1, readout2, r0, rl, ppm1, ppm2):
    ratio1 = ((5.0*rl)/(readout1*(5.0/1023))-rl)/r0
    ratio2 = ((5.0*rl)/(readout2*(5.0/1023))-rl)/r0
    ratioSum = ratio1+ratio2
    ratioLogSum = math.log10(ratio1) + math.log10(ratio2)

    ppm1_log = math.log10(ppm1)
    ppm2_log = math.log10(ppm2)
    ppm_sum = ppm1_log + ppm2_log
    ppmsq_sum = (ppm1_log**2) + (ppm2_log**2)

    ratioPPMSum = (ppm1_log*math.log10(ratio1))+(ppm2_log*math.log10(ratio2))

    meanPPM = ppm_sum / 2
    meanRatio = ratioLogSum / 2

    m = (ratioPPMSum - 2 * meanPPM * meanRatio) / (ppmsq_sum - 2 * meanPPM * meanPPM)
    b = meanRatio - m * meanPPM
    print("b : ", b)
    print("m : ", m)

def f(x, o):
    return abs((x-o)**2)

def d(x, h, o):
    return round((f(x + h, o) - f(x, o))/h)

def calc(o, h, lr):
    c = 370
    if c != o:
        c -= (d(c, h, o)*lr)
        print(c)
    else:
        print(c)

r = input("RL/10BIT/INTERSECT/GRADIENT/PPM/MB/DESCENT : ")

if r == "RL":
    readout = float(input("10BIT DATA : "))
    ppm = float(input("PPM : "))
    m = float(input("M : "))
    b = float(input("B : "))
    r0 = float(input("R0 : "))
    calculateRL(ppm, readout, m, b, r0)

elif r == "DESCENT":
    o = float(input("O : "))
    h = float(input("H : "))
    lr = float(input("LR : "))
    calc(o, h, lr)

elif r == "MB":
    g = input("3/2 DATA : ")
    if g == "3":
        readout1 = float(input("10BIT DATA 1 : "))
        readout2 = float(input("10BIT DATA 2 : "))
        readout3 = float(input("10BIT DATA 3 : "))
        ppm1 = float(input("PPM 1 : "))
        ppm2 = float(input("PPM 2 : "))
        ppm3 = float(input("PPM 3 : "))
        r0 = float(input("R0 : "))
        rl = float(input("RL : "))
        calculateBoth(readout1, readout2, readout3, r0, rl, ppm1, ppm2, ppm3)
    else:
        readout1 = float(input("10BIT DATA 1 : "))
        readout2 = float(input("10BIT DATA 2 : "))
        ppm1 = float(input("PPM 1 : "))
        ppm2 = float(input("PPM 2 : "))
        r0 = float(input("R0 : "))
        rl = float(input("RL : "))
        calculateBothTwoData(readout1, readout2, r0, rl, ppm1, ppm2)

elif r == "INTERSECT":
    readout = float(input("10BIT DATA : "))
    ppm = float(input("PPM : "))
    m = float(input("M : "))
    rl = float(input("RL : "))
    r0 = float(input("R0 : "))
    calculateB(ppm, readout, m, r0, rl)

elif r == "GRADIENT":
    readout = float(input("10BIT DATA : "))
    ppm = float(input("PPM : "))
    b = float(input("B : "))
    rl = float(input("RL : "))
    r0 = float(input("R0 : "))
    calculateM(ppm, readout, b, r0, rl)
    
elif r == "PPM":
    readout = float(input("10BIT DATA : "))
    rl = float(input("RL : "))
    m = float(input("M : "))
    b = float(input("B : "))
    r0 = float(input("R0 : "))
    g = input("LOG/LIN : ")

    if g == "LOG":
        calculatePPMLog(readout, m, b, r0, rl)
    else:
        calculatePPMLin(readout, m, b, r0, rl)

elif r =="10BIT":
    rl = float(input("RL : "))
    ppm = float(input("PPM : "))
    m = float(input("M : "))
    b = float(input("B : "))
    r0 = float(input("R0 : "))
    g = input("LOG/LIN : ")

    if g == "LOG":
        calculateLog(rl, ppm, m, b, r0)
    else:
        calculateLin(rl, ppm, m, b, r0)

else:
    print("INVALID")