#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <cstdlib>
#include <unordered_map>

using namespace std;

__interface IMobs
{
    virtual string ToString() = 0;
    virtual void BattleRoar() = 0;
};

class Archer : public IMobs
{
public:
    void BattleRoar() override
    {
        cout << "Лучник издает боевой клич!" << endl;
    }
    string ToString()
    {
        return "Лучник";
    }
};

class Mage : public IMobs
{
public:
    void BattleRoar() override
    {
        cout << "Маг издает боевой клич!" << endl;
    }
    string ToString()
    {
        return "Маг";
    }
};

class Knight : public IMobs
{
public:
    void BattleRoar() override
    {
        cout << "Рыцарь издает боевой клич!" << endl;
    }
    string ToString()
    {
        return "Рыцарь";
    }
};

class Necromancer : public IMobs
{
public:
    void BattleRoar() override
    {
        cout << "Некромант издает боевой клич!" << endl;
    }
    string ToString()
    {
        return "Некромант";
    }
};

__interface IMobsFactory
{
    virtual string ToString() = 0;
    virtual IMobs* Create() = 0;
};

class EasyLvlMobsFactory : public IMobsFactory
{
public:
    IMobs* Create() override
    {
        auto mobs = make_unique<vector<string>>(
            initializer_list<string> {
            "Лучник",
                "Маг",
                "Рыцарь"
        });

        int randIndex = rand() % mobs->size();
        string mobType = mobs->at(randIndex);

        if (mobType == "Лучник")
        {
            return new Archer();
        }
        else if (mobType == "Маг")
        {
            return new Mage();
        }
        else if (mobType == "Рыцарь")
        {
            return new Knight();
        }
        else
        {
            throw invalid_argument("Неизвестный тип моба");
        }
    }
    string ToString()
    {
        return "Легкий уровень";
    }
};

class MidleLvlMobsFactory : public IMobsFactory
{
public:
    IMobs* Create() override
    {
        if (m_hardMobsCount < 10)
        {
            m_hardMobsCount++;
            return new Knight();
        }

        // Создать объект EasyLvlMobsFactory.
        auto* easyFactory = new EasyLvlMobsFactory();

        // Создать объект IMobs с использованием фабрики.
        auto* mobs = easyFactory->Create();

        // Удалить объект фабрики.
        delete easyFactory;

        return mobs;
    }
    string ToString()
    {
        return "Средний уровень";
    }

private:
    int m_hardMobsCount = 0;
};


class HardLvlMobsFactory : public IMobsFactory
{
public:
    IMobs* Create() override
    {
        vector<string> mobs = {
            "Маг",
            "Рыцарь",
            "Некромант"
        };

        int randIndex = rand() % mobs.size();
        string mobType = mobs[randIndex];

        if (mobType == "Маг")
        {
            return new Mage();
        }
        else if (mobType == "Рыцарь")
        {
            return new Knight();
        }
        else if (mobType == "Некромант")
        {
            return new Necromancer();
        }
        else
        {
            throw invalid_argument("Неизвестный тип моба");
        }
    }

    string ToString()
    {
        return "Сложный уровень";
    }
};


class Tree
{
};

class Hero
{
};

class Battlefield
{
private:
    vector<IMobs*> mobs;
    vector<Tree*> trees;
    size_t mobsCount = 0;
public:
    Battlefield(vector<IMobs*> mobs, vector<Tree*> trees)
    {
        this->mobs = mobs;
        this->trees = trees;
        this->mobsCount = mobs.size();
    }

    void MobInfo()
    {
        unordered_map<string, int> mobsCount;

        for (auto& mob : mobs)
        {
            mobsCount[mob->ToString()]++;
        }

        for (const auto& mob : mobsCount)
        {
            cout << mob.first << ": " << mob.second << endl;
        }
    }
};

class Game
{
private:
    Battlefield* location;
    Hero* gamer;
public:
    Game(IMobsFactory& factory)
    {
        gamer = new Hero();

        Tree* trees[] = {
                new Tree(),
                new Tree()
        };

        vector<IMobs*> mobs;
        for (int i = 0; i < 100; i++)
        {
            mobs.push_back(factory.Create());
        }
        location = new Battlefield(mobs, vector<Tree*>(trees, trees + 2));
    }
    void StartGame()
    {
        cout << "Локация загружена" << endl;
        cout << "Деревья расставлены" << endl;
        cout << "Монстры на местах" << endl;
        cout << "Игрок загружен" << endl;
        cout << "И игра начинается" << endl;

        location->MobInfo();
    }
};

int main()
{
    setlocale(LC_ALL, "Russian");
    IMobsFactory* lvls[] = {
        new EasyLvlMobsFactory(),
        new MidleLvlMobsFactory(),
        new HardLvlMobsFactory()
    };

    do {
        cout << "Выберите уровень сложности и введите его номер (или 4 для выхода):" << endl;

        for (int i = 0; i < 3; i++)
        {
            cout << i + 1 << ". " << lvls[i]->ToString() << endl;
        }
        string lvlNumberStr;
        getline(cin, lvlNumberStr);
        int lvlNumber = stoi(lvlNumberStr) - 1;

        if (lvlNumber == -1 || lvlNumber == 3) {
            // Закрытие программы
            break;
        }
        if (lvlNumber >= -1 && lvlNumber < 3)
        {
            Game game(*lvls[lvlNumber]);
            game.StartGame();
            cout << endl;
        }
        else
        {
            cout << "Неправильный ввод. Выберите номер из списка." << endl;
        }
    } while (true);

    for (int i = 0; i < 3; i++)
    {
        delete lvls[i];
    }
    return 0;
}
