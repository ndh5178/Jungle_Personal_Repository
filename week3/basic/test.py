
def simplifyPath(path):
    """
    :type path: str
    :rtype: str
    """
    stack=[]
    result=[]
    for i in path:
        stack.append(i)
    stack=stack[::-1]
    j=0
    for i in range(len(stack)):
        if j == len(stack):
            break
        if j==len(stack)-1:
            result.append(stack[i])
            continue
        if stack[j]=="/"and j==0:
            j+=1
            continue
        if stack[j]=="/"and stack[j+1]=="/":
            j+=1
            continue
        if stack[j]=="."and stack[j+1]=="." and stack[j+2]=="/" and j==len(stack)-3:
            return "/"
        if stack[j]=="."and stack[j+1]=="." and stack[j+2]=="/":
            j=j+3
            while stack[j] != "/":
                j+=1
            continue
        if stack[j]=="."and stack[j+1]=="/":
            j=j+1
            continue
        result.append(stack[i])
        j+=1
    result=result[::-1]
    result = ''.join(result)

    return result
print(simplifyPath("/home/"))   
print(simplifyPath("/home//foo/"))  
print(simplifyPath("/home/user/Documents/../Pictures"))  
print(simplifyPath("/../"))  
print(simplifyPath("/.../a/../b/c/../d/./")) 