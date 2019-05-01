
#include <bits/stdc++.h>
using namespace std;

const int FIELD = 150, SOVIET_SOLDIERS=5, GERMAN_SOLDIERS=3, SOVIET_TANKS=1, GERMAN_TANKS=2, ARTILLERY=2, ZONE=10;
const int LIVES_DEFAULT = 50, GUN_DEFAULT = 5, TANK_DEFAULT = 10, ARTILLERY_DEFAULT=20;
const double GUN_DEVIAFION_DEFAULT = 0.03, ARTILLERY_DISPERSION_FUNCTION = 0.03, DEFAULT_SQUARE = 1;
const int TANK_DURATION = 2, ARTILLERY_DURATION = 10;

class Unit{
    public:
        virtual void go(int dx, int dy) = 0;
        virtual void shoot(int target_x, int target_y) const = 0;
        virtual Unit* clone() const = 0;
};

class Infantry : public Unit{
    friend bool operator !=(const Infantry& a, const Infantry& b);
    public:
        int x;
        int y;
        int lives;
        int gun_power;
        double gun_deviation_percent;
        Infantry(){
            x = rand()%ZONE + 1;
            y = rand()%(FIELD+1);
            lives = LIVES_DEFAULT;
            gun_power = GUN_DEFAULT;
            gun_deviation_percent = GUN_DEVIAFION_DEFAULT;
        }
        void go(int dx, int dy){};
        void shoot(int target_x, int target_y) const {};
        Infantry* clone() const{
            return new Infantry(*this);
        }
};

class Tank : public Unit{
    friend bool operator !=(const Tank& a, const Tank& b);
    public:
        int x;
        int y;
        int lives;
        int gun_power;
        double deviation_percent;
        int ticks_to_next_shoot;
        Tank(){
            x = rand()%ZONE + 1;
            y = rand()%(FIELD+1);
            lives = LIVES_DEFAULT;
            gun_power = TANK_DEFAULT;
            deviation_percent = GUN_DEVIAFION_DEFAULT*2.;
            ticks_to_next_shoot=0;
        }
        void go(int dx, int dy){};
        void shoot(int target_x, int target_y) const {};
        Tank* clone() const{
            return new Tank(*this);
        }
};

class Artillery: public Unit{
    friend bool operator !=(const Artillery& a, const Artillery& b);
    public:
        int x;
        int y;
        int lives;
        int gun_power;
        double dispersion;
        double square;
        int ticks_to_next_shoot;
        Artillery(){
            x = 0;
            y = rand()%(FIELD+1);
            lives = LIVES_DEFAULT;
            gun_power = ARTILLERY_DEFAULT;
            square = DEFAULT_SQUARE;
            dispersion = ARTILLERY_DISPERSION_FUNCTION;
            ticks_to_next_shoot=0;
        }
        void shoot(int target_x, int target_y) const {};
        void go(int dx, int dy){
            assert(0);
        }
        Artillery* clone() const{
            return new Artillery(*this);
        }
};

class Army{
    friend bool operator ==(const Army& a, const Army& b);
    private:
        vector<Infantry*> infantry;
        vector<Tank*> tanks;
        vector<Artillery*> artillery;
    public:
        //vector<Artillery*> artillery;
        void action(){

        }
        void add_soldiers(vector<Infantry*> added){
            for (Infantry* soldier: added){
                infantry.push_back(soldier);
            }
        }
        void add_tank(vector<Tank*> added){
            for (Tank* tank: added){
                tanks.push_back(tank);
            }
        }
        void add_artillery(vector<Artillery*> added){
            for (Artillery* artillery_unit: added){
                artillery.push_back(artillery_unit);
            }
        }
        ~Army(){
            infantry.clear();
            tanks.clear();
            artillery.clear();
        }
};

bool operator !=(const Infantry& a, const Infantry& b){
    if (a.lives != b.lives || a.gun_deviation_percent != b.gun_deviation_percent || a.gun_power != b.gun_power) return true;
    return false;
}

bool operator !=(const Tank& a, const Tank& b){
    if (a.lives != b.lives || a.deviation_percent != b.deviation_percent || a.gun_power != b.gun_power || a.ticks_to_next_shoot != b.ticks_to_next_shoot) return true;
    return false;
}

bool operator !=(const Artillery& a, const Artillery& b){
    if (a.lives != b.lives || a.dispersion != b.dispersion || a.square!=b.square || a.gun_power != b.gun_power || a.ticks_to_next_shoot != b.ticks_to_next_shoot) return true;
    return false;
}

bool operator ==(const Army& a, const Army& b)
{
    if (a.infantry.size() != b.infantry.size() || a.tanks.size() != b.tanks.size() || a.artillery.size() != b.artillery.size()) return false;
    vector<Infantry*> inf_a, inf_b;
    for (Infantry* unit : a.infantry) inf_a.push_back(unit);
    for (Infantry* unit : b.infantry) inf_b.push_back(unit);
    for (int i=0; i < inf_a.size(); i++){
        if ((*inf_a[i]) != (*inf_b[i])) return false;
    }
    vector<Tank*> tank_a, tank_b;
    for (Tank* unit : a.tanks) tank_a.push_back(unit);
    for (Tank* unit : b.tanks) tank_b.push_back(unit);
    for (int i=0; i < tank_a.size(); i++){
        if ((*tank_a[i]) != (*tank_b[i])) return false;
    }
    vector<Artillery*> art_a, art_b;
    for (Artillery* unit : a.artillery) art_a.push_back(unit);
    for (Artillery* unit : b.artillery) art_b.push_back(unit);
    for (int i=0; i < art_a.size(); i++){
        if ((*art_a[i]) != (*art_b[i])) return false;
    }
    return true;
}

class ArmyFactory{
    public:
        virtual vector<Infantry*> create_infantry() = 0;
        virtual vector<Tank*> create_tanks() = 0;
        virtual vector<Artillery*> create_artillery() = 0;
        //virtual ~ArmyFactory() = 0;
};

class SovietArmyFactory : public ArmyFactory{
    public:
        vector<Infantry*> create_infantry(){
            vector<Infantry*> arr;
            for (int i=0; i < SOVIET_SOLDIERS; i++){
                arr.push_back(new Infantry());
            }
            return arr;
        }
        vector<Tank*> create_tanks(){
            vector<Tank*> arr;
            for (int i=0; i < SOVIET_TANKS; i++){
                arr.push_back(new Tank());
            }
            return arr;
        }
        vector<Artillery*> create_artillery(){
            vector<Artillery*> arr;
            for (int i=0; i < ARTILLERY; i++){
                arr.push_back(new Artillery());
            }
            return arr;
        }
};

class GermanArmyFactory : public ArmyFactory{
    public:
        vector<Infantry*> create_infantry(){
            vector<Infantry*> arr;
            for (int i=0; i < GERMAN_SOLDIERS; i++){
                arr.push_back(new Infantry());
                arr.back()->x = FIELD-arr.back()->x;
            }
            return arr;
        }
        vector<Tank*> create_tanks(){
            vector<Tank*> arr;
            for (int i=0; i < GERMAN_TANKS; i++){
                arr.push_back(new Tank());
                arr.back()->x = FIELD-arr.back()->x;
            }
            return arr;
        }
        vector<Artillery*> create_artillery(){
            vector<Artillery*> arr;
            for (int i=0; i < ARTILLERY; i++){
                arr.push_back(new Artillery());
                arr.back()->x = FIELD-arr.back()->x;
            }
            return arr;
        }
};

class Game{
    public:
        Army* create_army(ArmyFactory &factory){
            Army* new_army = new Army();
            new_army->add_soldiers(factory.create_infantry());
            new_army->add_tank(factory.create_tanks());
            new_army->add_artillery(factory.create_artillery());
            return new_army;
        }
};

int run(){
    Game game;
    SovietArmyFactory s_factory;
    Army* soviet_army = game.create_army(s_factory);
    GermanArmyFactory g_factory;
    Army* german_army = game.create_army(g_factory);
    cout << "Something`s been created" << endl;
    return 0;
}

