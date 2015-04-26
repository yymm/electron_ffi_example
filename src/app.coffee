ref = require 'ref'
ffi = require 'ffi'
Struct = require 'ref-struct'
ArrayType = require 'ref-array'
util = require './dist/util.js'

w = 800
h = 600
n = 10


IntPtr = ref.refType("int")

Point = Struct
    'x': 'double'
    'y': 'double'

PointArray = ArrayType(Point)

NodeTriangle = Struct
    'p1': Point
    'p2': Point
    'p3': Point

NodeTriangleArray = ArrayType(NodeTriangle)

Circle = Struct
    'center': Point
    'radius': 'double'

CircleArray = ArrayType(Circle)

delaunay = ffi.Library './native/libdelaunay.so',
    'calculate': [ 'int', [ 'int', 'int', 'int', PointArray, IntPtr, NodeTriangleArray, IntPtr, CircleArray ] ]

points = new PointArray(n)
tn = ref.alloc('int')
triangles = new NodeTriangleArray(100*n)
cn = ref.alloc('int')
circles = new CircleArray(10*n)

for i in [0..n-1]
    points[i].x = Math.random() * w
    points[i].y = Math.random() * h

delaunay.calculate(w, h, points.length, points, tn, triangles, cn, circles)


out_tn = tn.deref()
#console.log out_tn

out_cn = cn.deref()
#console.log out_cn

cv = document.getElementById('cv')
ctx = cv.getContext('2d')

cv.width = w
cv.height = h

for i in [0..n-1]
    util.drawPoint(ctx, points[i])

for i in [0..out_tn-1]
    #console.log triangles[i].p1.x
    #console.log triangles[i].p2.x
    #console.log triangles[i].p3.x
    util.drawTriangle(ctx, triangles[i])

for i in [0..out_cn-1]
    util.drawCircle(ctx, circles[i])
