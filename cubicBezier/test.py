# -*- coding: utf-8 -*-
import sys
import os
path = os.path.realpath(os.path.split(__file__)[0])
path = os.path.split(path)[0]
sys.path.append(path)
import cubicBezier.lib as cubicBezier
import matplotlib.pyplot as plt


def draw_cubicBezier(plt, points, color='b', curve=True, controlPoint=True,
tangent=True):
    '''
    cubicBezier描画用関数
    Args:
        plt: matplotlib.pyplot
        points list(p1, p2, p3, p4): cubicBezierのポイント。※[始点、制御点、制御点、終点]

    Returns:
        None
    '''
    cpoints = cubicBezier.sampling_cubicBezier(points, 30)
    x = [i[0] for i in cpoints]
    y = [i[1] for i in cpoints]

    if tangent:
        p0 = points[0]
        p1 = points[1]
        p2 = points[2]
        p3 = points[3]
        plt.plot([p0[0], p1[0]], [p0[1], p1[1]], color, markersize=15)
        plt.plot([p2[0], p3[0]], [p2[1], p3[1]], color, markersize=15)
    
    if controlPoint:
        for i in points:
            plt.plot([i[0]], [i[1]], color, marker='.', markersize=25)
        
    if curve:
        plt.plot(x, y, color)

def draw_grid(plt, count, x1, x2, y1):
    '''
    グリッドの描画
    Args:
        plt: matplotlib.pyplot
        x1 float:描画始点のx座標
        x2 float:描画終点のx座標
        x1 float:描画y軸
    Returns:
        None
    '''
    # draw grid
    vecA=(x2-x1)/count
    xg=[vecA*i+x1 for i in range(1, count+1)]
    plt.xticks(xg)
    plt.yticks([y1])
    plt.grid()
    

    
    
# テスト用ポイント
p=[]
p.append([0,0])
p.append([30, -110])
p.append([70, 100])
p.append([100,100])


count=30
points = cubicBezier.sampling_cubicBezier(p, count)

# draw
draw_grid(plt, 5, points[0][0], points[-1][0], points[0][1])
#描画用に整理
x = [i[0] for i in points]
y = [i[1] for i in points]

px = [i[0] for i in points]
py = [i[1] for i in points]
for i in zip(x, y):
    plt.plot( i[0], i[1], 'r', marker='|', markersize=30)
    plt.plot( i[0], i[1], 'black', marker='.', markersize=5)
    
draw_cubicBezier(plt, p, controlPoint=False, tangent=False)
#plt.plot(px, py, 'b' )
plt.show()
