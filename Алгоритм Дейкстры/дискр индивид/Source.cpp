#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

bool ERR = false; // детектор ошибок

int number(string s); // string -> int
void input_all(int** graph, ifstream& input, int* distance, bool* is_it_checked, int* line, int n); // функция инициализации всех массивов
void start_dijkstra(int n, int** graph, int* distance, bool* is_it_checked, int* line); // алгоритм дейкстры
void check_a_b(int& a, int& b, int n, ifstream& input); // проверка вершин на корректность
void check_n(int& n, ifstream& input); // проверка размерности на корректность
bool correct(string s); // проверка числа на корректность
void way(int n, int a, int b, int* line, ofstream& output);// нахождение пути

int main()
{
	ofstream MyFile("filename.txt");
	unsigned int startTime = clock();
	ifstream input; input.open("input.txt");// входной ф-л
	ofstream output("output.txt"); //  выходной ф-л
	int n; // размерность графа
	check_n(n, input);// ввод кол-ва вершин графа
	if (ERR) { output << "ERROR"; return 1; } // если находим ошибку - очищаем память и выходим из прогр.
	int** graph; // исходный массив расстояний
	graph = new int* [n];
	for (int i = 0; i < n; i++)
		graph[i] = new int[n];
	int* distance = new int[n]; // матрица расстояний до вершин графа
	bool* is_it_checked = new bool[n]; // проверена ли вершина
	int* line = new int[n];
	input_all(graph, input, distance, is_it_checked, line, n); // инициализация всех массивов
	if (ERR) { output << "ERROR"; return 1; }
	int a, b; // нужные нам вершины
	check_a_b(a, b, n, input); // ввод нужных нам вершин
	if (ERR) { output << "ERROR"; goto ende; } // если находим ошибку - очищаем память и выходим из прогр.
	distance[b - 1] = 0;
	start_dijkstra(n, graph, distance, is_it_checked, line);
	output << distance[a - 1] << endl;
	way(n, a, b, line, output);

	//------------------//
	//  очистка памяти  //
	//------------------//
ende:
	output.close(); input.close();
	delete[] is_it_checked;
	delete[] distance;
	for (int i = 0; i < n; i++)
		delete graph[i];
	delete[] graph;
	unsigned int endTime = clock();
	cout << (endTime - startTime) << endl;
	return 1;
}

void input_all(int** graph, ifstream& input, int* distance, bool* is_it_checked, int* line, int n)
{
	int temp_n = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			string temp; input >> temp;
			if (temp == "~")
			{
				graph[i][j] = INT_MAX / 10;
				temp_n++;
			}
			else
				if (correct(temp))
				{
					graph[i][j] = number(temp);
					temp_n++;
				}
				else { ERR = true; return; }
			if (i > j && graph[j][i] != graph[i][j]) ERR = true;
		}
		if (graph[i][i] != 0) { ERR = true; return; }
	}

	if (temp_n != n * n) { ERR = true; return; }

	for (int i = 0; i < n; i++)
	{
		line[i] = -10;
		is_it_checked[i] = false;
		distance[i] = INT_MAX / 10;
	}
}

int number(string s)
{
	return stoi(s);
}

void start_dijkstra(int n, int** graph, int* distance, bool* is_it_checked, int* line)
{
	for (int k = 0; k < n; k++)
	{
		int current_distance = INT_MAX / 10;
		int current_index = -1;
		for (int i = 0; i < n; i++)
			if (distance[i] < current_distance && !is_it_checked[i])
			{
				current_distance = distance[i];
				current_index = i;
			}
		for (int i = 0; i < n; i++)
			if (graph[current_index][i] != 0)
			{
				int temp = current_distance + graph[current_index][i];
				if (temp < distance[i])
				{
					distance[i] = temp;
					line[i] = current_index;
				}
			}
		is_it_checked[current_index] = true;
	}

}


void check_a_b(int& a, int& b, int n, ifstream& input)
{
	string sa, sb;
	input >> sa >> sb;
	for (int i = 0; i < sa.length(); i++)
		if (!(sa[i] >= '0' && sa[i] <= '9'))
			ERR = true;
	for (int i = 0; i < sb.length(); i++)
		if (!(sb[i] >= '0' && sb[i] <= '9'))
			ERR = true;
	a = number(sa); b = number(sb);
	if (a <= 0 || b <= 0) ERR = true;
	if (a > n || b > n) ERR = true;
	if (a == b) ERR = true;
}

void check_n(int& n, ifstream& input)
{
	string sn;
	input >> sn;
	for (int i = 0; i < sn.length(); i++)
		if (!(sn[i] >= '0' && sn[i] <= '9'))
			ERR = true;
	n = number(sn);
	if (n <= 0) ERR = true;
}

bool correct(string s)
{
	for (int i = 0; i < s.length(); i++)
		if (!(s[i] >= '0' && s[i] <= '9'))
			return false;
	int temp = number(s);
	if (temp < 0) return false;
	return true;
}

void way(int n, int a, int b, int* line, ofstream& output)
{
	output << a << ' ';
	int temp = a - 1;
	while (line[temp] != -10)
	{
		temp = line[temp];
		output << temp + 1 << ' ';
	}
}
