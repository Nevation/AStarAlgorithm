#include <iostream>
#include <fstream>
#include <ostream>
#include <vector>
#include <map>
#include <set>
using namespace std;

typedef pair<int, int> point;
typedef pair<double, double> dist;

void print_error(string error) {
	cout << error << endl;
}

void print_pos(point k) {
	printf("Point: <%d, %d>\n", k.first, k.second);
}
struct AstarList {
	double GScore;
	double HScore;
	point ParentPos;
};

typedef pair<int, AstarList> alist;

class AstarAlgorithm {
public:
	AstarAlgorithm();
	AstarAlgorithm(pair<char, char> p0int, pair<char, char> map);
	~AstarAlgorithm() { map_info.clear(); }
	bool ExecuteAlgorithm();
protected:
	void print_now(point pos);
	void print_list(vector<point> pos);
	char GetMapChar(point pos);
	bool ASTAR();
	bool SetMap();
	bool IsInMap(point pos);
	dist GetGHScore(point pos);
	double GetFScore(dist v);
	bool SaveResult(vector<point> pos);
private:
	vector<vector<char>> map_info;
	point pos_start;		// start position
	point pos_end;			// end position
	pair<char, char>  char_point;	// first start char, second end char
	pair<char, char>  char_map;		// first open block, second close block
	map<point, AstarList> OpenList;  // first FScore, second List
	multimap<point, AstarList> CloseList;	  // first pos, second List
};



AstarAlgorithm::AstarAlgorithm()
{

}

AstarAlgorithm::AstarAlgorithm(pair<char, char> p0int, pair<char, char> map_info)
{
	char_point = p0int;
	char_map = map_info;
}

bool AstarAlgorithm::SetMap()
{
	ifstream file("map.txt");
	if (!file.is_open()) {
		print_error("None File");
		return false;
	}
	vector<char> line;
	char ch;
	while (true) {
		ch = file.get();
		if (ch == '\n' || ch == -1) {
			map_info.push_back(line);
			line.clear();
			if (ch == -1) break;
		}
		else {
			line.push_back(ch);
			if (ch == char_point.first)	 pos_start = make_pair(map_info.size(), line.size() - 1);
			else if (ch == char_point.second) pos_end = make_pair(map_info.size(), line.size() - 1);
		}
	}
	file.close();
	return true;
}

bool AstarAlgorithm::IsInMap(point pos)
{
	if (pos.first < 0 || pos.second < 0) return false;
	if (pos.first >= (int)map_info.size() || pos.second >= (int)map_info[0].size()) return false;
	return true;
}

dist AstarAlgorithm::GetGHScore(point pos)
{
	dist result; // first GSocre second HScore

	result.first = OpenList[pos].GScore;
	result.second = sqrt(pow(pos.first - pos_end.first, 2) + pow(pos.second - pos_end.second, 2));

	return result;
}

double AstarAlgorithm::GetFScore(dist v)
{
	return v.first + v.second;
}

bool AstarAlgorithm::SaveResult(vector<point> pos)
{
	ofstream file("result.txt");
	if (!file.is_open()) {
		print_error("None File");
		return false;
	}
	for (int i = 0; i < map_info.size(); i++) {
		for (int j = 0; j < map_info[i].size(); j++) {
			point p = make_pair(i, j);
			bool check = false;
			for (auto k : pos) {
				if (p == k) {
					file << "A";
					check = true;
				}
			}
			if (!check)
				file << map_info[i][j];
		}
		file << "\n";
	}
	file.close();
	return true;
}

bool AstarAlgorithm::ExecuteAlgorithm()
{
	if (!SetMap()) {
		print_error("Error Set map_info");
		return false;
	}
	
	if (!ASTAR()) {
		print_error("Error A Star");
		return false;
	}
}

void AstarAlgorithm::print_now(point pos)
{
	for (int i = 0; i < map_info.size(); i++) {
		for (int j = 0; j < map_info[i].size(); j++) {
			point k = make_pair(i, j);
			if (pos == k) {
				printf("A");
			}
			else if (k == pos_start) {
				printf("S");
			}
			else if (OpenList.count(k) != 0) {
				printf("E");
			}
			else if (CloseList.count(k) != 0) {
				printf("W");
			}
			else {
				printf("%c", map_info[i][j]);
			}
		}
		printf("\n");
	}
}

void AstarAlgorithm::print_list(vector<point> pos)
{
	for (int i = 0; i < map_info.size(); i++) {
		for (int j = 0; j < map_info[i].size(); j++) {
			point p = make_pair(i, j);
			bool check = false;
			for (auto k : pos) {
				if (p == k) {
					printf("A");
					check = true;
				}

			}
			if (!check) 
				printf("%c", map_info[i][j]);
			
		}
		printf("\n");
	}
}

char AstarAlgorithm::GetMapChar(point pos)
{
	return map_info[pos.first][pos.second];
}

void print_open(map<point, AstarList> List) {
	for (auto k : List) {
		printf("POS: <%02d, %02d> G: %.2lf S: %.2lf PAR: <%02d, %02d>\n",
			k.first.first, k.first.second, k.second.GScore, k.second.HScore, k.second.ParentPos.first, k.second.ParentPos.second);
	}
	printf("\n\n\n\n");
}
void print_close(multimap<point, AstarList> List) {
	for (auto k : List) {
		printf("POS: <%02d, %02d> G: %.2lf S: %.2lf PAR: <%02d, %02d>\n",
			k.first.first, k.first.second, k.second.GScore, k.second.HScore, k.second.ParentPos.first, k.second.ParentPos.second);
	}
	printf("\n\n\n\n");
}
bool cmp_point(point a, point b) {
	return a.first == b.first && a.second == b.second;
}

void prints(pair<point, AstarList> k) {
	printf("POS: <%02d, %02d> G: %.2lf S: %.2lf PAR: <%02d, %02d>\n",
		k.first.first, k.first.second, k.second.GScore, k.second.HScore, k.second.ParentPos.first, k.second.ParentPos.second);

}
bool AstarAlgorithm::ASTAR()
{
	AstarList Root = { 0, INT8_MAX, make_pair(-1, -1) };
	pair<point, AstarList> it= make_pair(pos_start, Root);
	CloseList.insert(it);


	OpenList.clear();

		while (true) {
			point pos = it.first;
			point parent_pos = it.second.ParentPos;


			if (it.first == pos_end) {
				break;
			}

			AstarList adder;
			double fscore = 0;
			adder.ParentPos.first = pos.first;
			adder.ParentPos.second = pos.second;

			
			for (int x = -1; x < 2; x++) {
				for (int y = -1; y < 2; y++) {
					if (x == 0 && y == 0) continue;


					point this_pos = make_pair(pos.first + x, pos.second + y);

					if ((IsInMap(this_pos) && GetMapChar(this_pos) != char_map.second && !cmp_point(this_pos, parent_pos)) &&
						((CloseList.count(this_pos) == 0))) {
						
						double second = sqrt(pow(this_pos.first - pos_end.first, 2) + pow(this_pos.second - pos_end.second, 2));
						// guess dist to end
						
						adder.GScore = it.second.GScore + 1;
						// real dist from start

						adder.HScore = second;

						if (OpenList.find(this_pos) != OpenList.end()) {
							if (OpenList[this_pos].GScore + OpenList[this_pos].HScore > adder.GScore + adder.HScore) {
								OpenList.erase(this_pos);
							}
						}

						OpenList.insert(make_pair(this_pos, adder));


					}

				}
			}

			double min = 99999999.0;
			
			for (auto iter : OpenList) {
				fscore = iter.second.GScore + iter.second.HScore;
				if (fscore < min) {
					min = fscore;
					it.first = iter.first;
					it.second.GScore = iter.second.GScore;
					it.second.HScore = iter.second.HScore;
					it.second.ParentPos.first = iter.second.ParentPos.first;
					it.second.ParentPos.second = iter.second.ParentPos.second;
				}
			}

			OpenList.erase(it.first);
			CloseList.insert(it);


		}
	
	
	auto root = it;
	vector<point> path;
	while (true) {
		path.push_back(root.first);
		//prints(root);
		auto s = CloseList.find(root.second.ParentPos);
		double max = 99999999.0;
		int find_cnt = CloseList.count(root.second.ParentPos);
		pair<point, AstarList> save;

		for (int i = 0; i < find_cnt; i++) {
			double sum = s->second.GScore + s->second.HScore;
			if (max > sum) {
				max = sum;
				save.first = s->first;
				save.second.GScore = s->second.GScore;
				save.second.HScore = s->second.HScore;
				save.second.ParentPos = s->second.ParentPos;
			}
			s++;
		}

		if (save.first == pos_start) break;
		
		root.first = save.first;
		root.second.GScore = save.second.GScore;
		root.second.HScore = save.second.HScore;
		root.second.ParentPos = save.second.ParentPos;
	}

	if (SaveResult(path)) {
		cout << "Success Save!" << endl;
	}
	

	return true;
}



int main() {
	// (start, end), (open, close)
	AstarAlgorithm astar(make_pair('S', 'F'), make_pair('.', 'o'));
	astar.ExecuteAlgorithm();

	return 0;
}