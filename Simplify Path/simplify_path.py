from collections import deque

class Solution:
    def simplifyPath(self, path: str) -> str:
        d = deque()
        path = path.split("/")
        for elem in path:
            if len(d) > 0 and elem == "..":
                d.pop()
            elif elem != "" and elem != "." and elem != ".." :
                d.append(elem)
        
        ans = "/"
        while len(d) > 0:
            ans += d.popleft()
            ans += "/"
            
        if len(ans) == 1:
            return ans
        else:
            return ans[:-1]
