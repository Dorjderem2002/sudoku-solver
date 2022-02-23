#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdio>
#include <iostream>
#include <string>
#include <map>
using namespace sf;

struct my_vector {
	int x,y;
};

bool check_row(std::vector<int> v) {
	std::map<int,int> mp;
	for(int i=0;i<v.size();i++) {
		mp[v[i]]++;
		if(mp[v[i]] > 1 && v[i] > 0) {
			return false;
		}
	}
	return true;
}

bool check_col(std::vector< std::vector<int> > v,int index) {
	std::map<int,int> mp;
	for(int i=0;i<v.size();i++) {
		mp[v[i][index]]++;
		if(mp[v[i][index]] > 1 && v[i][index] > 0) {
			return false;
		}
	}
	return true;
}

void increment(my_vector &pos, int margin) {
	if(pos.x < margin) {
		pos.x++;
	}
	else if(pos.y < margin){
		pos.y++;
		pos.x = 0;
	}
	else {
		pos.x = 0;
		pos.y = 0;
	}
}

void decrement(my_vector &pos, int margin) {
	if(pos.x > 0) {
		pos.x--;
	}
	else if(pos.y > 0){
		pos.y--;
		pos.x = margin - 1;
	}
	else {
		std::cout<<"error out of bonds"<<std::endl;
	}
}

int main() {
	int win_size = 540;
	RenderWindow win(VideoMode(win_size,win_size),"hello world");
	std::vector<std::vector<RectangleShape> > vec(9, std::vector<RectangleShape> (9));
	int cell_size = win_size / 9;

	std::string game_data[] = {"??4?5????",
							 "9??7346??",
							 "??3?21?49",
							 "?35?9?48?",
							 "?9?????3?",
							 "?76?1?92?",
							 "31?97?2??",
							 "??9182??3",
							 "????6?1??"};


	RectangleShape highlight;
	highlight.setSize(Vector2f(cell_size,cell_size));
	highlight.setFillColor(Color(0,0,0,100));
	highlight.setPosition(Vector2f(0,0));

	Font font;
	font.loadFromFile("Arial.ttf");
	std::vector< std::vector<Text> > numbers(9, std::vector<Text> (9));
	std::vector< std::vector<int> > num_data(9, std::vector<int> (9,0));

	for(int i=0;i<numbers.size();i++) {
		for(int j=0;j<numbers[i].size();j++) {
			numbers[i][j].setFont(font);
			numbers[i][j].setPosition(Vector2f(j * cell_size + 18, i * cell_size + 5));
			numbers[i][j].setCharacterSize(40);
			numbers[i][j].setColor(Color::Black);
			if(game_data[i][j] != '?') {
				numbers[i][j].setString(std::to_string(game_data[i][j] - 48));
				num_data[i][j] = game_data[i][j] - 48;
			}
		}
	}
	

	for(int i=0;i<vec.size();i++) {
		for(int j=0;j<vec[i].size();j++) {
			RectangleShape t;
			t.setSize(Vector2f(cell_size,cell_size));
			t.setFillColor(Color::White);
			t.setPosition(Vector2f(j * cell_size, i * cell_size));
			t.setOutlineThickness(1);
			t.setOutlineColor(Color::Black);
			vec[i][j] = t;
		}
	}

	RectangleShape l1,l2,l3,l4;

	l1.setFillColor(Color::Black);
	l1.setSize(Vector2f(win_size,2));
	l1.setPosition(Vector2f(0,win_size / 3));
	l2 = l1; l3 = l1; l4 = l1;
	l2.setPosition(Vector2f(0,win_size / 3 * 2));
	l3.setSize(Vector2f(2,win_size));
	l3.setPosition(Vector2f(win_size / 3,0));
	l4 = l3;
	l4.setPosition(Vector2f(win_size / 3 * 2,0));
	my_vector curr_pos = {0,0};


	//Backtrack
	bool okay = true;

	bool finished = true;

	while(win.isOpen()) {
		Event e;
		
		while(win.pollEvent(e)) {
			if(e.type == Event::Closed) {
				win.close();
			}
			if(e.type == Event::MouseButtonPressed) {
				if(e.mouseButton.button == Mouse::Left) {
					curr_pos.y = e.mouseButton.y / cell_size;
					curr_pos.x = e.mouseButton.x / cell_size;
				}
			}
			if(e.type == Event::KeyPressed) {
				if((e.key.code > 26 && e.key.code < 36)) {
					int temp_num = e.key.code - 26;
					num_data[curr_pos.y][curr_pos.x] = temp_num;
					numbers[curr_pos.y][curr_pos.x].setString(std::to_string(temp_num));
					
					for(int i=0;i<num_data.size();i++) {
						if(!check_row(num_data[i])) {
							for(int j=0;j<vec[i].size();j++) {
								vec[i][j].setFillColor(Color::Red);
							}
							okay = false;
						}
						else {
							for(int j=0;j<vec[i].size();j++) {
								vec[i][j].setFillColor(Color::White);
							}
						}
						if(!check_col(num_data, i)) {
							for(int j=0;j<vec[i].size();j++) {
								vec[j][i].setFillColor(Color::Red);
							}
							okay = false;
						}
						else {
							for(int j=0;j<vec[i].size();j++) {
								vec[i][j].setFillColor(Color::White);
							}
						}
					}
				}
				else if(e.key.code == Keyboard::Space) {
					finished = !finished;
				}
			}
		}
		//backtrack logic
		for(int h=0;h<50;h++) {
			if(!finished) {
			if(game_data[curr_pos.y][curr_pos.x] == '?') {
				num_data[curr_pos.y][curr_pos.x]++;
				if(num_data[curr_pos.y][curr_pos.x] > vec.size()) {
					num_data[curr_pos.y][curr_pos.x] = 0;
					numbers[curr_pos.y][curr_pos.x].setString("");
					decrement(curr_pos, vec.size());
					while(game_data[curr_pos.y][curr_pos.x] != '?') {
						decrement(curr_pos, vec.size());
					}	
				}
				else {
					numbers[curr_pos.y][curr_pos.x].setString(std::to_string(num_data[curr_pos.y][curr_pos.x]));
					bool local_okay = true;
					for(int i=0;i<num_data.size();i++) {
						for(int j=0;j<vec[i].size();j++) {
							vec[i][j].setFillColor(Color::White);
						}
					}
					for(int i=0;i<num_data.size();i++) {
						if(!check_row(num_data[i])) {
							for(int j=0;j<vec[i].size();j++) {
								vec[i][j].setFillColor(Color::Red);
							}
							local_okay = false;
						}
						if(!check_col(num_data, i)) {
							for(int j=0;j<vec[i].size();j++) {
								vec[j][i].setFillColor(Color::Red);
							}
							local_okay = false;
						}
					}
					
					if(local_okay && (curr_pos.x == vec.size()-1 && curr_pos.y == vec.size()-1)) {
						finished = true;
						curr_pos.x = vec.size() - 1;
						curr_pos.y = vec.size() - 1;
					}
					else if(local_okay) {
						increment(curr_pos, vec.size());
					}
				}
			}
			else {
				increment(curr_pos,vec.size());
			}
		}
		}
		

		
		
		

		highlight.setPosition(curr_pos.x * cell_size, curr_pos.y * cell_size);

		//board draw
		win.clear();
		for(int i=0;i<vec.size();i++) {
			for(int j=0;j<vec[i].size();j++) {
				win.draw(vec[i][j]);
				win.draw(numbers[i][j]);
			}
		}

		//selected box
		win.draw(highlight);

		//box border
		win.draw(l1);
		win.draw(l2);
		win.draw(l3);
		win.draw(l4);

		win.display();
	}
	return 0;
}