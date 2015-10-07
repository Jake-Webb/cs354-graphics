#include <vector>

using namespace std;

class Vertex {
	public:
		float x;
		float y;
		float z;
		float normal[3];
		int adjacentFaces;

		Vertex() {
			normal[0] = 0.0;
			normal[1] = 0.0;
			normal[2] = 1.0;
			adjacentFaces = 0;
		}

		Vertex(float vs[]) {
			x = vs[0];
			y = vs[1];
			z = vs[2];
			normal[0] = 0.0;
			normal[1] = 0.0;
			normal[2] = 1.0;
			adjacentFaces = 0;
		}

		void addUpNormal(float n[]) {
			normal[0] += n[0];
			normal[1] += n[1];
			normal[2] += n[2];
			++adjacentFaces;
		}

		void finalizeNormal() {
			normal[0] /= adjacentFaces;
			normal[1] /= adjacentFaces;
			normal[2] /= adjacentFaces;
		}
};

class Face {
	public:
		vector<Vertex*> vs;
		float normal[3];

		Face(Vertex tvs[]) {
			vs.push_back(tvs);
			vs.push_back(tvs+1);
			vs.push_back(tvs+2);
			normal[0] = 0.0;
			normal[1] = 0.0;
			normal[2] = 1.0;
		}

		void calculateNormal() {
			float u[3] = {vs[1]->x - vs[0]->x, vs[1]->y - vs[0]->y, vs[1]->z - vs[0]->z};
			float v[3] = {vs[2]->x - vs[0]->x, vs[2]->y - vs[0]->y, vs[2]->z - vs[0]->z};
			normal[0] = (u[1] * v[2]) - (u[2] * v[1]);
			normal[1] = (u[2] * v[0]) - (u[0] * v[2]);
			normal[2] = (u[0] * v[1]) - (u[1] * v[0]);

			vs[0]->addUpNormal(normal);
			vs[1]->addUpNormal(normal);
			vs[2]->addUpNormal(normal);
		}
};

class Trimesh {
	public:
		vector<Vertex> vs;
		vector<Face> fs;

		Trimesh(){
			vs[0] = Vertex();
		}

		void addVertex(float tvs[]) {
			Vertex v = Vertex(tvs);
			vs.push_back(v);
		}

		void addFace(int vi[]) {
			int i = vi[0];
			int j = vi[1];
			int k = vi[2];
			Vertex tvs[3] = {vs[i], vs[j], vs[k]};
			Face f = Face(tvs);
			fs.push_back(f);
		}

		void calculateNormals() {
			int i = 0;
			for(i; i < fs.size(); ++i)
				fs[i].calculateNormal();
			i = 1;
			for(i; i < vs.size(); ++i)
				vs[i].finalizeNormal();
		}
};