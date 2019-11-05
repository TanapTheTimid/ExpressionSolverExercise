def eval(exp):
    #algorithm cannot handle 2 signs next to each other
    exp = exp.replace("+-","-")

    if "(" in exp:
        #first open paren
        start = exp.find("(")
        
        #find the corresponding closing paren by keeping track of num of open parens until 0 is reached
        numparen = 1
        i = start
        while(numparen > 0):
            i += 1
            if(exp[i] == "("):
                numparen += 1
            elif(exp[i] == ")"):
                numparen -= 1
        
        #the last closure = index of paren
        end = i
        
        subexp = exp[start+1:end] # cut paren
        evalsub = eval(subexp) #eval the subexpression in paren
        
        #shove it back into the algorithm to evaluate correct order after paren delt with
        return eval(exp[:start] + str(evalsub) + exp[end+1:])
        
    #lowest priority operator becomes highest priority separator
    #separate based on reverse order operation because separation means being evaluated last
    if "+" in exp or "-" in exp:
        divpnt = exp.rfind("+")
        divpnt2 = exp.rfind("-")
        divpnt = max(divpnt, divpnt2) #plus or minus whichever one appears last
        
        if(exp[divpnt] == "+"):
            return eval(exp[:divpnt]) + eval(exp[divpnt+1:])
        else:
            return eval(exp[:divpnt]) - eval(exp[divpnt+1:])
        
    if "*" in exp or "/" in exp:
        divpnt = exp.rfind("*")
        divpnt2 = exp.rfind("/")
        divpnt = max(divpnt, divpnt2)
        
        if(exp[divpnt] == "*"):
            return eval(exp[:divpnt]) * eval(exp[divpnt+1:])
        else:
            return eval(exp[:divpnt]) / eval(exp[divpnt+1:])
        
    if "^" in exp:
        divpnt = exp.find("^") #order of operation is defined weirdly for exponents, back to front
        return eval(exp[:divpnt]) ** eval(exp[divpnt+1:])
        
    #finally once simplify to number, return float
    return float(exp)
        

print(eval("3-5-8+3+(3-6)+((2^4)^2+3/2-3*5+((3)))+3*((3+2)+(3+4+(3/4^4)^2/4+(3)))/2+(3-3)*2-3+(3/4*((3)/4))+((3))/2+(3-334444)*2*3+-5+2+(3-3)*2+2+(3+4)+3+(2+3.234234125412234234234234234)*3+2/4/(4/4)+3"))
