// Test_test.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "../include/include/nlohmann/json.hpp"
#include <thread>
#include <map>
#include <mutex>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;

using json = nlohmann::json;

using namespace std;

mutex mtx;
map<int, string> moves{
	{true, "ping"},
	{false, "pong"}
};

void player(int size, int num_player, int num_team, bool& turn) {

	lock_guard<mutex> guard(mtx);

	//mtx.lock();

	cout << num_player << '/' << size << ", "
		<< num_team << ": " << moves[turn] << '\n';

	turn = !turn;

	//mtx.unlock();
}

int main()
{
	setlocale(LC_ALL, "RU");
	
	vector<vector <thread>> vec(2);
	vector<thread> temp;

	int team_s = 0,
		prev_team_s = 0;
	bool game_over = false,
		turn = true;

	while (true)
	{
		ifstream f("config.json");
		json data = json::parse(f);
		game_over = data["game_over"];
		if (game_over) {
			cout << "--------\t" << "THE GAME IS OVER\t" << "--------\t" << endl;
			break;
		}
		else {
			team_s = data["team_size"];		

			if (team_s < 0) {
				cout << "--------\t" << "The number of players cannot be less than zero\t" 
					<< "--------\t" << endl;
			}
			else if (team_s == 0) {
				continue;
			}
			else
			{
				
				for (int i = 1; i <= 2; i++) 
				{
					for (int j = 1; j <= team_s; j++) 
					{
						vec[i - 1].push_back(thread(player, team_s, j, i, std::ref(turn)));
						vec[i - 1][j - 1].join();
					}
					vec[i-1].clear();
				}
			}
		}
	}

	return 0;
}


