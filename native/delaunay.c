#include "delaunay.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	Point start;
	Point end;
} Edge;

typedef struct {
	int id;
	Point points[3];
	Edge edges[3];
} Triangle;

typedef struct {
	int n;
	int length;
	Triangle* elem;
} List;

void push_triangle(List* list, Point p[3]);
void remove_triangle(List* list, int id);

typedef struct {
	int length;
	Edge* elem;
} Stack;

void push(Stack* s, Edge* e);
Edge* pop(Stack* s);
int stack_length(Stack* s);

int getCircumscribedCircle(Triangle* t, Circle* circle);
int hitCircle(Circle* circle, Point* p);
int searchSharedEdgeTriangle(List* triangles, Edge* edge, Triangle shared_triangles[2]);

int hasEdge(Triangle* t, Edge* e);
int isSameEdge(Edge* e1, Edge* e2);
int isSamePoint(Point* p1, Point* p2);
int hasPointByTriangle(Triangle* t, Point* p);
int isSameTriangle(Triangle* t1, Triangle* t2);
int hasPointByEdge(Edge* e, Point* p);
int getCommonPointEdge(Triangle* t, Edge* edge);
int getOtherEdge(Triangle* t, Edge* edge, Edge other_edge[]);

int calculate(int width,
			  int height,
			  int n,
			  Point points[],
			  int* tn,
			  NodeTriangle final_triangles[],
			  int* cn,
			  Circle circles[]) {
	int i, j, hit_count = 0;
	Point p[3], a, b, c, p1[3], p2[3], p3[3], p_A, p_B, p_C, p_D;
	Triangle *t_abc, *t_abd;
	Circle temp_cir;

	Stack edge_stack;
	edge_stack.length = 0;
	edge_stack.elem = (Edge*)malloc(sizeof(Edge)*10*n);
	Edge* edge;
	Edge other_edge[2];

	Triangle edge_triangles[2];
	Triangle hit_triangles[100];

	List triangles;
	triangles.n = 0;
	triangles.length = 0;
	triangles.elem = (Triangle*)malloc(sizeof(Triangle)*100*n);

	(*tn) = 0;
	(*cn) = 0;

	// super triangle
	double cx = width / 2.0;
	double cy = height / 2.0;
	double r = sqrt(width*width + height*height) / 2;
	p[0].x = cx - sqrt(3)*r; p[0].y = cy - r;
	p[1].x = cx + sqrt(3)*r; p[1].y = cy - r;
	p[2].x = cx;             p[2].y = 2*r + cy;

	push_triangle(&triangles, p);

	for (i = 0; i < n; ++i) {
		for (j = 0; j < triangles.length; ++j) {
			getCircumscribedCircle(&triangles.elem[j], &temp_cir);
			if (hitCircle(&temp_cir, &points[i]) != 0) {
				hit_triangles[hit_count] = triangles.elem[j];
				hit_count++;
			}
		}
		/*printf("[%d]: %d (%f, %f: %f)vs(%f, %f)\n", i, hit_count, temp_cir.center.x, temp_cir.center.y, temp_cir.radius, points[i].x, points[i].y);*/
		for (j = 0; j < hit_count; ++j) {
			push(&edge_stack, &hit_triangles[j].edges[0]);
			push(&edge_stack, &hit_triangles[j].edges[1]);
			push(&edge_stack, &hit_triangles[j].edges[2]);

			remove_triangle(&triangles, hit_triangles[j].id);

			a = hit_triangles[j].points[0];
			b = hit_triangles[j].points[1];
			c = hit_triangles[j].points[2];

			p1[0] = a; p1[1] = b; p1[2] = points[i];
			p2[0] = b; p2[1] = c; p2[2] = points[i];
			p3[0] = c; p3[1] = a; p3[2] = points[i];

			push_triangle(&triangles, p1);
			push_triangle(&triangles, p2);
			push_triangle(&triangles, p3);
		}

		// Debug print
		/*printf("loop: i=%d, hit=%d, len=%d\n", i, hit_count, triangles.length);*/
		/*for (j = 0; j < triangles.length; ++j) {*/
			/*printf("[%d]: %d, (% 5.2f, % 5.2f) (% 5.2f, % 5.2f) (% 5.2f, % 5.2f)\n", j, triangles.elem[j].id,*/
					/*triangles.elem[j].points[0].x, triangles.elem[j].points[0].y,*/
					/*triangles.elem[j].points[1].x, triangles.elem[j].points[1].y,*/
					/*triangles.elem[j].points[2].x, triangles.elem[j].points[2].y);*/
		/*}*/

		hit_count = 0;
		while(stack_length(&edge_stack) != 0) {
			edge = pop(&edge_stack);
			if (searchSharedEdgeTriangle(&triangles, edge, edge_triangles) < 0) continue;
			
			t_abc = &edge_triangles[0];
			t_abd = &edge_triangles[1];

			if (isSameTriangle(t_abc, t_abd) != 0) {
				remove_triangle(&triangles, t_abc->id);
				remove_triangle(&triangles, t_abd->id);
				continue;
			}

			p_A = edge->start;
			p_B = edge->end;
			p_C = t_abc->points[getCommonPointEdge(t_abc, edge)];
			p_D = t_abd->points[getCommonPointEdge(t_abd, edge)];
			/*printf("A(%f, %f), B(%f, %f), C(%f, %f), D(%f, %f)\n",*/
					/*p_A.x, p_A.y,*/
					/*p_B.x, p_B.y,*/
					/*p_C.x, p_C.y,*/
					/*p_D.x, p_D.y*/
			/*);*/

			getCircumscribedCircle(t_abc, &temp_cir);

			if (hasPointByTriangle(t_abc, &p[0]) == 0 &&
				hasPointByTriangle(t_abc, &p[1]) == 0 &&
				hasPointByTriangle(t_abc, &p[2]) == 0) {
				circles[(*cn)].center = temp_cir.center;
				circles[(*cn)].radius = temp_cir.radius;
				(*cn)++;
			}

			if (hitCircle(&temp_cir, &p_D) != 0) {
				// Debug print
				printf("flip: id[%d], id[%d]\n", t_abc->id, t_abd->id);
				printf("edge: start(%f, %f), end(%f, %f)\n", edge->start.x, edge->start.y, edge->end.x, edge->end.y);

				remove_triangle(&triangles, t_abc->id);
				remove_triangle(&triangles, t_abd->id);

				p1[0] = p_A; p1[1] = p_C; p1[2] = p_D;
				p2[0] = p_B; p2[1] = p_C; p2[2] = p_D;

				push_triangle(&triangles, p1);
				push_triangle(&triangles, p2);

				getOtherEdge(t_abc, edge, other_edge);
				push(&edge_stack, &other_edge[0]);
				push(&edge_stack, &other_edge[1]);
				getOtherEdge(t_abd, edge, other_edge);
				push(&edge_stack, &other_edge[0]);
				push(&edge_stack, &other_edge[1]);
			}
		}
	}

	for (i = 0; i < triangles.length; ++i) {
		if (hasPointByTriangle(&triangles.elem[i], &p[0]) != 0 ||
			hasPointByTriangle(&triangles.elem[i], &p[1]) != 0 ||
			hasPointByTriangle(&triangles.elem[i], &p[2]) != 0) {
			continue;
		}
		final_triangles[(*tn)].p1 = triangles.elem[i].points[0];
		final_triangles[(*tn)].p2 = triangles.elem[i].points[1];
		final_triangles[(*tn)].p3 = triangles.elem[i].points[2];
		(*tn)++;
	}

	free(edge_stack.elem);
	free(triangles.elem);

	return 0;
}

void push_triangle(List* list, Point p[3]) {
	Triangle t;
	t.points[0] = p[0];
	t.points[1] = p[1];
	t.points[2] = p[2];
	t.edges[0].start = p[0];
	t.edges[0].end   = p[1];
	t.edges[1].start = p[1];
	t.edges[1].end   = p[2];
	t.edges[2].start = p[2];
	t.edges[2].end   = p[0];

	t.id = list->n;
	list->elem[list->length] = t;
	list->n++;
	list->length++;
}

void remove_triangle(List* list, int id) {
	int i, j = 0;
	for (i = 0; i < list->length; ++i) {
		if (list->elem[i].id == id) {
			break;
		}
	}
	if (list->length - 1 != i) {
		for (j = i+1; j < list->length; ++j) {
			list->elem[j-1] = list->elem[j];
		}
	}
	list->length--;
}

int getCircumscribedCircle(Triangle* t, Circle* circle) {
	double x1 = t->points[0].x;
	double y1 = t->points[0].y;
	double x2 = t->points[1].x;
	double y2 = t->points[1].y;
	double x3 = t->points[2].x;
	double y3 = t->points[2].y;


	double x1_2 = x1 * x1;
	double x2_2 = x2 * x2;
	double x3_2 = x3 * x3;
	double y1_2 = y1 * y1;
	double y2_2 = y2 * y2;
	double y3_2 = y3 * y3;
	
	double c = 2 * ((x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1));
	double x = ((y3 - y1) * (x2_2 - x1_2 + y2_2 - y1_2) + (y1 - y2) * (x3_2 - x1_2 + y3_2 - y1_2)) / c;
	double y = ((x1 - x3) * (x2_2 - x1_2 + y2_2 - y1_2) + (x2 - x1) * (x3_2 - x1_2 + y3_2 - y1_2)) / c;
	double _x = (x1 - x);
	double _y = (y1 - y);
	double r = sqrt((_x * _x) + (_y * _y));

	circle->center.x = x;
	circle->center.y = y;
	circle->radius = r;

	return 0;
}

int hitCircle(Circle* circle, Point* p) {
	double x = p->x - circle->center.x;
	double y = p->y - circle->center.y;
	double len = sqrt(x*x + y*y);
	if (len < circle->radius) {
		return 1;	// in
	}
	return 0;	// out
}

int searchSharedEdgeTriangle(List* list, Edge* e, Triangle tarr[2]) {
	int i, cnt = 0;
	for (i = 0; i < list->length; ++i) {
		if (hasEdge(&list->elem[i], e) != 0) {
			tarr[cnt] = list->elem[i];
			cnt++;
			if (cnt >= 2) break;
		}
	}
	if (cnt < 2) return -1;
	return 0;
}

int hasEdge(Triangle* t, Edge* e) {
	if (isSameEdge(&t->edges[0], e) != 0 ||
		isSameEdge(&t->edges[1], e) != 0 ||
		isSameEdge(&t->edges[2], e) != 0) {
		return 1;
	}
	return 0;
}

int isSameEdge(Edge* e1, Edge* e2) {
	if ((isSamePoint(&e1->start, &e2->start) != 0 && isSamePoint(&e1->end, &e2->end) != 0) ||
		(isSamePoint(&e1->start, &e2->end) != 0 && isSamePoint(&e1->end, &e2->start) != 0))
		return 1;
	return 0;
}

int isSamePoint(Point* p1, Point* p2) {
	if (p1->x == p2->x && p1->y == p2->y) return 1;
	return 0;
}

int hasPointByTriangle(Triangle* t, Point* p) {
	int i = 0;
	for (i = 0; i < 3; ++i) {
		if (isSamePoint(&t->points[i], p) != 0) {
			return 1;
		}
	}
	return 0;
}

int isSameTriangle(Triangle* t1, Triangle* t2) {
	int i = 0;
	for (i = 0; i < 3; ++i) {
		if (hasPointByTriangle(t2, &t1->points[i]) == 0) {
			return 0;
		}
	}
	return 1;
}

int hasPointByEdge(Edge* e, Point* p) {
	if ((e->start.x == p->x && e->start.y == p->y) ||
		(e->end.x == p->x && e->end.y == p->y))
		return 1;
	return 0;
}

int getCommonPointEdge(Triangle* t, Edge* e) {
	int i = 0;
	for (i = 0; i < 3; ++i) {
		if (hasPointByEdge(e, &t->points[i]) == 0) {
			return i;
		}
	}
	return -1;
}

int getOtherEdge(Triangle* t, Edge* edge, Edge other_edge[]) {
	int i, cnt = 0;
	for (i = 0; i < 3; ++i) {
		if (isSameEdge(edge, &t->edges[i]) != 0) {
			other_edge[cnt] = t->edges[i];
			cnt++;
		}
	}
	return -1;
}

void push(Stack* s, Edge* e) {
	s->elem[s->length] = *e;
	s->length++;
}

Edge* pop(Stack* s) {
	if (s->length == 0) {
		return NULL;
	}
	s->length--;
	return &s->elem[s->length];
}

int stack_length(Stack* s) {
	return s->length;
}
