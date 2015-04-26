var ArrayType, Circle, CircleArray, IntPtr, NodeTriangle, NodeTriangleArray, Point, PointArray, Struct, circles, cn, ctx, cv, delaunay, ffi, h, i, j, k, l, m, n, out_cn, out_tn, points, ref, ref1, ref2, ref3, ref4, tn, triangles, util, w;

ref = require('ref');

ffi = require('ffi');

Struct = require('ref-struct');

ArrayType = require('ref-array');

util = require('./dist/util.js');

w = 800;

h = 600;

n = 10;

IntPtr = ref.refType("int");

Point = Struct({
  'x': 'double',
  'y': 'double'
});

PointArray = ArrayType(Point);

NodeTriangle = Struct({
  'p1': Point,
  'p2': Point,
  'p3': Point
});

NodeTriangleArray = ArrayType(NodeTriangle);

Circle = Struct({
  'center': Point,
  'radius': 'double'
});

CircleArray = ArrayType(Circle);

delaunay = ffi.Library('./native/libdelaunay.so', {
  'calculate': ['int', ['int', 'int', 'int', PointArray, IntPtr, NodeTriangleArray, IntPtr, CircleArray]]
});

points = new PointArray(n);

tn = ref.alloc('int');

triangles = new NodeTriangleArray(100 * n);

cn = ref.alloc('int');

circles = new CircleArray(10 * n);

for (i = j = 0, ref1 = n - 1; 0 <= ref1 ? j <= ref1 : j >= ref1; i = 0 <= ref1 ? ++j : --j) {
  points[i].x = Math.random() * w;
  points[i].y = Math.random() * h;
}

delaunay.calculate(w, h, points.length, points, tn, triangles, cn, circles);

out_tn = tn.deref();

out_cn = cn.deref();

cv = document.getElementById('cv');

ctx = cv.getContext('2d');

cv.width = w;

cv.height = h;

for (i = k = 0, ref2 = n - 1; 0 <= ref2 ? k <= ref2 : k >= ref2; i = 0 <= ref2 ? ++k : --k) {
  util.drawPoint(ctx, points[i]);
}

for (i = l = 0, ref3 = out_tn - 1; 0 <= ref3 ? l <= ref3 : l >= ref3; i = 0 <= ref3 ? ++l : --l) {
  util.drawTriangle(ctx, triangles[i]);
}

for (i = m = 0, ref4 = out_cn - 1; 0 <= ref4 ? m <= ref4 : m >= ref4; i = 0 <= ref4 ? ++m : --m) {
  util.drawCircle(ctx, circles[i]);
}
