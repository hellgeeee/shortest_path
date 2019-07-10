// route_task.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
using namespace std;

namespace route_task {

	#define INF 0xfffffff

	void numeric_in(unsigned int& variable) {
		cin >> variable;

		if (cin.fail())
		{
			// user didn't input a number
			cout << "Wrong enter, try again, please. \n";
			cin.clear(); // reset failbit
			cin.ignore(100, '\n'); //skip bad input (idk why 100 though)
			// next, request user reinput
			numeric_in(variable);
		}
	}

	enum Line {
		Red = 0,
		Blue = 1,
		Green = 2,
		Undefined = 3
	};

	enum Direction {
		Towards = 0,
		Backwards = 1
	};

	struct Distance {
		int _val;
		Direction _dir;
		Line _line;
		Distance(const unsigned int& v, const Direction& d, const Line& l):_val(v), _dir(d), _line(l) {};
	};

	struct Station {

		Station(const string& name) { const_cast<string&>(_name) = name; }

		void addBranch(const unsigned int& branch_id, const unsigned int& distance, const Direction& dir, const Line& l) {
			_branches.insert(pair<unsigned int, Distance>(branch_id, Distance(distance, dir, l)));
		}

		void out(const string& prefix = "  ->  ") {
			cout << prefix << _name;
		}
		
		void clear() {
			_current_path.clear();
			_current_distance = INF;
		}

		void setCurrentDistance(const unsigned int& dist_to_origin, const vector<unsigned int>& path_to_origin) {
			_current_distance = dist_to_origin;
			_current_path = path_to_origin;
		}

		// short-term properties
		unsigned int _current_distance = INF; //destination from departure point
		vector<unsigned int> _current_path; //destination from departure point
		map<unsigned int, Distance> _branches; // linked station and distance to it
		map<Line, unsigned int> schedule_item; // line and time
		// permanent properties
		const string _name = "Noname";
	};

	struct Schedule{

		unsigned int timeToWait(const Direction& dir, const Line& line, const unsigned int& arrival_t, const unsigned int& time_now) {

			// time moments of passing towards: n*T + t = time_now + waiting_time
				// waiting_time = n*T + t - time_now = n*T + t - time_now%T
					// = t - time_now%T , if t - time_now%T >= 0 
					// = t - time_now%T + T, if t - time_now%T < 0 
			// time moments of passing backwards: n*T + (T - t) = time_now + waiting_time
					// = (T-t) - time_now%T , if t - time_now%T >= 0 
					// = (T-t) - time_now%T + T, if t - time_now%T < 0
			// where T is time of train cycle/2, t is arrival time to station by schedule
			int wait_time = 0;
			if (dir == Towards) {
				int wait_time = arrival_t - time_now % trainCycle(line);
			}
			else {
				int wait_time = (trainCycle(line) - arrival_t) - time_now % trainCycle(line);
			}
			if (wait_time < 0) wait_time += trainCycle(line);
			return wait_time;
		}

		unsigned int trainCycle(const Line& line) { // sec
			switch (line) {
				case Red: return _red_cycle;										// assumed to be difined by user
				case Blue: return _blue_cycle;
				case Green: return _green_cycle;
				default: return 100000;
			}
		}

		void setTrainCycle(const Line& line, const unsigned int& val) {
			switch (line) {
				case Red: const_cast<unsigned int&>(_red_cycle) = val;
				case Blue: const_cast<unsigned int&>(_blue_cycle) = val;
				case Green: const_cast<unsigned int&>(_green_cycle) = val;
			}
		}

		const unsigned int _red_cycle = 40000; // actually it's time duration of cycle/2.
		const unsigned int _blue_cycle = 40000;
		const unsigned int _green_cycle = 40000;
	};

	class Researcher {
	public:

		Researcher() { introduction(); }
		~Researcher() { 
			for (auto s = _stations_pool.begin(); s != _stations_pool.end(); ++s) delete s->second; 
		}

		void introduction() {
			cout << "Hello, nice to see you.\n\nWhat time is it? Enter minutes passed since 0:00\n";
			numeric_in(_time_start);

			initializeStations();

			cout << "\nCheck out list of all underground stations are available.\n";
			for (auto it = _stations_pool.begin(); it != _stations_pool.end(); ++it) {
				cout << setw(5) << it->first; 
				it->second->out(" : "); cout << "\n";
			}
		}

		void initializeStations() {
			// entered in code now, assumed to be entered by user later
			unsigned int id = 223;
			unsigned int t = 0, dist = 500, cycle = 0; // assumed to be entered by user later
			Line line = Red;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Kamennaja Horka")));
			_stations_pool[id]->addBranch(222, dist, Backwards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id--;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Kuncauscyna")));
			_stations_pool[id]->addBranch(223, dist, Towards, line);
			_stations_pool[id]->addBranch(221, dist, Backwards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;
			
			id--;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Spartyunaja")));
			_stations_pool[id]->addBranch(222, dist, Towards, line);
			_stations_pool[id]->addBranch(220, dist, Backwards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id--;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Puskinskaja")));
			_stations_pool[id]->addBranch(221, dist, Towards, line);
			_stations_pool[id]->addBranch(219, dist, Backwards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id--;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Maladzionznaja")));
			_stations_pool[id]->addBranch(220, dist, Towards, line);
			_stations_pool[id]->addBranch(218, dist, Backwards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id--;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Frunzienskaja/Jubiliejnaja plosca")));
			_stations_pool[id]->addBranch(219, dist, Towards, line);
			_stations_pool[id]->addBranch(217, dist, Backwards, line);
			_stations_pool[id]->addBranch(315, dist, Backwards, Green);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(Green, dist*7));
			cycle += dist;

			id--;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Niamiha")));
			_stations_pool[id]->addBranch(218, dist, Towards, line);
			_stations_pool[id]->addBranch(216, dist, Backwards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id--;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Kastrycnickaja/Kupalauskaja")));
			_stations_pool[id]->addBranch(217, dist, Towards, line);
			_stations_pool[id]->addBranch(215, dist, Backwards, line);
			_stations_pool[id]->addBranch(115, dist, Backwards, Blue);
			_stations_pool[id]->addBranch(117, dist, Towards, Blue);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(Blue, dist*8));
			cycle += dist;

			id--;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Piefsamajskaja")));
			_stations_pool[id]->addBranch(216, dist, Towards, line);
			_stations_pool[id]->addBranch(214, dist, Backwards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id--;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Praliotarskaja")));
			_stations_pool[id]->addBranch(215, dist, Towards, line);
			_stations_pool[id]->addBranch(213, dist, Backwards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id--;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Traktarny zavod")));
			_stations_pool[id]->addBranch(214, dist, Towards, line);
			_stations_pool[id]->addBranch(212, dist, Backwards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id--;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Partyzanskaja")));
			_stations_pool[id]->addBranch(213, dist, Towards, line);
			_stations_pool[id]->addBranch(211, dist, Backwards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id--;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Autazavodskaja")));
			_stations_pool[id]->addBranch(212, dist, Towards, line);
			_stations_pool[id]->addBranch(210, dist, Backwards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id--;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Mahiliouskaja")));
			_stations_pool[id]->addBranch(211, dist, Towards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			_schedule.setTrainCycle(Red, cycle); cycle = 0;

			id = 109;
			t = 0;
			line = Blue;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Scomyslica")));
			_stations_pool[id]->addBranch(110, dist, Towards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id++;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Malinauka")));
			_stations_pool[id]->addBranch(109, dist, Backwards, line);
			_stations_pool[id]->addBranch(111, dist, Towards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id++;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Piatrouscyna")));
			_stations_pool[id]->addBranch(110, dist, Backwards, line);
			_stations_pool[id]->addBranch(112, dist, Towards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id++;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Michalova")));
			_stations_pool[id]->addBranch(111, dist, Backwards, line);
			_stations_pool[id]->addBranch(113, dist, Towards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id++;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Hrusauka")));
			_stations_pool[id]->addBranch(112, dist, Backwards, line);
			_stations_pool[id]->addBranch(114, dist, Towards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id++;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Instytut kultury")));
			_stations_pool[id]->addBranch(113, dist, Backwards, line);
			_stations_pool[id]->addBranch(115, dist, Towards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id++;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Plosca Lienina/Vakzalnaja")));
			_stations_pool[id]->addBranch(114, dist, Backwards, line);
			_stations_pool[id]->addBranch(216, dist, Towards, line);
			_stations_pool[id]->addBranch(313, dist, Backwards, Green);
			_stations_pool[id]->addBranch(315, dist, Towards, Green);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(Green, dist * 7));
			cycle += dist;

			id +=2;
			t += dist; 
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Plosca Pioramohi")));
			_stations_pool[id]->addBranch(216, dist, Backwards, Red);
			_stations_pool[id]->addBranch(118, dist, Towards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id++;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Plosca Jakuba Kolasa")));
			_stations_pool[id]->addBranch(117, dist, Backwards, line);
			_stations_pool[id]->addBranch(119, dist, Towards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id++;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Akademija navuk")));
			_stations_pool[id]->addBranch(118, dist, Backwards, line);
			_stations_pool[id]->addBranch(120, dist, Towards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id++;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Park Caliuskincau")));
			_stations_pool[id]->addBranch(119, dist, Backwards, line);
			_stations_pool[id]->addBranch(121, dist, Towards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id++;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Maskouskaja")));
			_stations_pool[id]->addBranch(120, dist, Backwards, line);
			_stations_pool[id]->addBranch(122, dist, Towards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id++;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Uschod")));
			_stations_pool[id]->addBranch(121, dist, Backwards, line);
			_stations_pool[id]->addBranch(123, dist, Towards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id++;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Barysauski trakt")));
			_stations_pool[id]->addBranch(122, dist, Backwards, line);
			_stations_pool[id]->addBranch(124, dist, Towards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id++;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Urucca")));
			_stations_pool[id]->addBranch(123, dist, Backwards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;
			cycle += dist;

			_schedule.setTrainCycle(Blue, cycle); cycle = 0;

			id = 310;
			t = 0;
			line = Green;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Slucki Hascinieca")));
			_stations_pool[id]->addBranch(311, dist, Towards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id++;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Niemarsanski Sad")));
			_stations_pool[id]->addBranch(310, dist, Backwards, line);
			_stations_pool[id]->addBranch(312, dist, Towards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id++;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Aeradromnaja")));
			_stations_pool[id]->addBranch(311, dist, Backwards, line);
			_stations_pool[id]->addBranch(313, dist, Towards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id++;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Kavaiskaja Slabada")));
			_stations_pool[id]->addBranch(312, dist, Backwards, line);
			_stations_pool[id]->addBranch(115, dist, Towards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;

			id+=2;
			t += dist;
			_stations_pool.insert(pair<int, Station*>(id, new Station("Plosca Fransliska Bahusovica")));
			_stations_pool[id]->addBranch(115, dist, Backwards, line);
			_stations_pool[id]->addBranch(218, dist, Towards, line);
			_stations_pool[id]->schedule_item.insert(pair<Line, unsigned int>(line, t));
			cycle += dist;
			cycle += dist;
			cycle += dist;

			_schedule.setTrainCycle(Green, cycle); cycle = 0;

			// Unkomment for testing!
			/* Schedule::setCycle(Red, 40000);
			Schedule::setCycle(Blue, 160000);
			Schedule::setCycle(Green, 40000);
			*/

		}

		void clear() {
			for (auto station = _stations_pool.begin(); station != _stations_pool.end(); ++station) {
				station->second->clear(); 
				_departure_point_id = 0;
				_destination_point_id = 0;
			}
		}

		void getInitialConditions() {

			clear();

			cout << "\nPlease enter a code of departure point:\n ";
			numeric_in(_departure_point_id);
			if (_stations_pool.find(_departure_point_id) == _stations_pool.end()) { // entry with that key doesn't exist
				cout << "\nSorry, it seems points with code you entered don't exist \nPlease try again!";
				getInitialConditions();
			}
			else {
				_stations_pool[_departure_point_id]->out();  cout << "\n";
			}

			cout << "Please enter a code of destination point:\n ";
			numeric_in(_destination_point_id);
			if (_stations_pool.find(_destination_point_id) == _stations_pool.end()) {
				cout << "\nSorry, it seems points with =code you entered don't exist \nPlease try again!";
				getInitialConditions();
			}
			else {
				_stations_pool[_destination_point_id]->out(); cout << "\n";
			}

			// exceptions handling
			if (_departure_point_id == _destination_point_id) { 
				cout << "\n You've been arrived. Enter another path.";  
				getInitialConditions();
			};
			vector<unsigned int> tmp_vec; 
			tmp_vec.push_back(_departure_point_id);
			_stations_pool[_departure_point_id]->setCurrentDistance(0, tmp_vec);//_current_distance = 0;
		}

		void outputRezult() {
			if		(_stations_pool.empty()) 
				cout << "\n It seems to be no way from your departure point to your destination. Call support department please\n +79179795733\n";
			else	
				cout << "\n Optimal path:\n You're here";
			for(int station_id : _stations_pool[_destination_point_id]->_current_path) {
				// output names of stations at found rout
				_stations_pool[station_id]->out();
			}
			cout <<"\n\n";
		}

		void search(Line current_line = Undefined, unsigned int current_point_id = 0/*uncomment for first method*/ ) {

			// first (whong) method of detouring (end to end all branches)
				if (!current_point_id) current_point_id = _departure_point_id;
				Station* cur_point = _stations_pool[current_point_id];

				// we use map for sorting to be automativeand effective
				map<unsigned int, pair<unsigned int, Line>> stations_to_detour; // detour_order (relate destination), station_id, line

				for (auto branch = cur_point->_branches.begin(); branch != cur_point->_branches.end(); ++branch) {

					unsigned int distance_assumed = cur_point->_current_distance + branch->second._val;
					Line new_line = branch->second._line;

					if (new_line != current_line) { // transfer
						Direction d = branch->second._dir;
						scheduleInfluence(distance_assumed, cur_point, _time_start, new_line, d);
					}

					unsigned int id = branch->first, detour_order = distance_assumed;
					if (findPointDistance(id, distance_assumed, cur_point->_current_path)) {
						// maintain uniqness of key for staions to overcome
						if (stations_to_detour.find(detour_order) != stations_to_detour.end()) detour_order++;
						stations_to_detour.insert(pair<unsigned int, pair<unsigned int, Line>>(detour_order, pair<unsigned int, Line>(id, new_line)));
					}
				}

				for (auto branch = stations_to_detour.begin(); branch != stations_to_detour.end(); ++branch) {
					search(branch->second.second, branch->second.first);
			}
			// end first method of detour


			// second (right) method of detouring (end to end all branches)
				// instead of recourive call
				// cycle initialization
				// <detour_order (relate destination), <station_id, line>>
/*				map<unsigned int, pair<unsigned int, Line>> detour_curent_level;
				detour_curent_level.insert(pair<unsigned int, pair<unsigned int, Line>>(0, pair<unsigned int, Line>(_departure_point_id, Undefined)));
				map<unsigned int, pair<unsigned int, Line>> detour_next_level;
				
				while (true) {

					// action of detouring

					// for each station of current level
					for (auto cur_node = detour_curent_level.begin(); cur_node != detour_curent_level.end(); cur_node++) {
						Station* cur_point = _stations_pool[cur_node->first];
						Line current_line = cur_node->second.second;

						// for each branch going out of it
						for (auto branch = cur_point->_branches.begin(); branch != cur_point->_branches.end(); ++branch) {

							unsigned int distance_assumed = cur_point->_current_distance + branch->second._val;
							Line new_line = branch->second._line;

							// if branch is of another line then station itself, consider delay for transfer
							if (new_line != current_line) { // if transfer
								Direction d = branch->second._dir;
								scheduleInfluence(distance_assumed, cur_point, _time_start, new_line, d);
							}

							// if our branch yiell distance less then it already has, we will accaunt it in next level (next while cycle)
							unsigned int id = branch->first, detour_order = distance_assumed;
							if (findPointDistance(id, distance_assumed, cur_point->_current_path)) {
								// maintain uniqness of key for staions to overcome
								if (detour_next_level.find(detour_order) != detour_next_level.end()) detour_order++; 
								detour_next_level.insert(pair<unsigned int, pair<unsigned int, Line>>(detour_order, pair<unsigned int, Line>(id, new_line)));
							}
						}
					}

					// hendle finish of detouring
					unsigned int min_distance_current = INF;
					for (auto next_node = detour_curent_level.begin(); next_node != detour_curent_level.end(); next_node++) {

						// find minimal distance of stations we will be considering next level
						if (_stations_pool[next_node->first]->_current_distance < min_distance_current)
							min_distance_current = _stations_pool[next_node->first]->_current_distance;
					}

					if (min_distance_current >= _stations_pool[_destination_point_id]->_current_distance) break;

					// next step detouring
					detour_curent_level = detour_next_level;
					detour_next_level.clear();
				}*/
		}

		bool findPointDistance(const unsigned int& station_id, const unsigned int& distance_assumed, const vector<unsigned int>& current_path) { // finds general distance from departure point to current point we observe

			if (distance_assumed < _stations_pool[station_id]->_current_distance) { 
				
				// then we continue existant path to new path
				vector<unsigned int> tmp_vec = current_path;
				tmp_vec.push_back(station_id);
				_stations_pool[station_id]->setCurrentDistance(distance_assumed, tmp_vec);
					
				// if we look at the station which is not destination
				if ( (station_id != _destination_point_id)
				// and if we have distance from origin to point we are looking at - less then from origin to destination
				&& (_stations_pool[station_id]->_current_distance < _stations_pool[_destination_point_id]->_current_distance) ){					
					return true;
				}
			}
			return false;
		}

		void scheduleInfluence(unsigned int& distance_assumed, Station* cur_point, unsigned int time_start, Line line_new, Direction d) {
			unsigned int arr_t = 0;
			if (cur_point->schedule_item.find(line_new) == cur_point->schedule_item.end())
				;// cout << "\n Sorry, it seems schedule was not set for station " << cur_point->_name << ". Schedule will not be accaunted while calculation";
			else arr_t = cur_point->schedule_item[line_new];
			distance_assumed += _schedule.timeToWait(d, line_new, arr_t, time_start + cur_point->_current_distance); // means "time of start + time on te way"
		}

	private:
		map<const unsigned int, Station*> _stations_pool;
		unsigned int _time_start = 0; // minutes since 0:00
		unsigned int _departure_point_id = 0;
		unsigned int _destination_point_id = 0;
		Schedule _schedule;
	};

}

int main()
{
	using namespace route_task;;
		Researcher researcher;
		researcher.initializeStations();
	while (true) {
		researcher.getInitialConditions();
		researcher.search();
		researcher.outputRezult();
		cout << "Do you want to try another route? [y/n]\n";
		char answer;
		cin >> answer;
		if (answer == 'y');
		else if (answer == 'n') exit(0);
		else  exit(0); // idk what to do with this
	}
}
