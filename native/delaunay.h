typedef struct {
	double x;
	double y;
} Point;

typedef struct {
	Point p1;
	Point p2;
	Point p3;
} NodeTriangle;

typedef struct {
	Point center;
	double radius;
} Circle;

int calculate(int width,
			  int height,
			  int n,
			  Point points[],
			  int* tn,
			  NodeTriangle final_triangles[],
			  int* cn,
			  Circle circles[]);
