#include <vector>

using namespace std;

class Vertex {
	public:
		float x;
		float y;
		float z;

		Vertex(float *vs) {
			x = vs[0];
			y = vs[1];
			z = vs[2];
		}
};

class Face {
	public:
		vector<Vertex> vs;
};

class Trimesh {
	public:
		vector<Vertex> vs;
		vector<Face> fs;

		Trimesh() {
			
		}

		void addVertex(float *vs) {
			Vertex v = Vertex(vs);
			vs.push_back(v);
		}

		void addFace(int *vs) {

		}
};