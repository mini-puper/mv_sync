//Синхронизация Map и Vector
//Исходя из задания, есть различные варианты реализации.
//Пропишу дополнительные условия, для конкретики:
//  - Изначально размер контейнеров одинаковый
//  - Каждый контенейнер заполняется своими случайными числами (Map != Vector)
//  - Значение контейнеров не сортируется, дубликаты  в каждом контейнере не удаляются

#include <vector>
#include <map>
#include <random>
#include <iostream>

using namespace std;

void randomMV(vector<int>& vec) {
  uniform_int_distribution<> dist(1, 9);
  random_device r;

  generate(vec.begin(), vec.end(), [&dist, &r] {return dist(r); });
}

void randomMV(map<int, int>& mapp, size_t size) {
  uniform_int_distribution<> dist(1, 9);
  random_device r;

  for (int i = 0; i < size; ++i) {
    mapp.emplace(i, dist(r));
  }
}

void coutMV(vector<int>& vec, map<int, int>& ma) {

  cout << "Vector\t";
  for (auto v : vec)
    cout << v << " ";
  cout << endl;

  cout << "Map\t";
  for (auto m : ma)
    cout << m.second << " ";
  cout << endl;

}

int main()
{
  setlocale(LC_ALL, "ru");

  uniform_int_distribution<> dist(5, 15);
  random_device r;

  //Исходный размер Map и Vector
  const int size = 16;

  vector<int> vec_sync(size);
  map<int, int> map_sync;

  // заполнение Map и Vector рандомными значениями
  randomMV(vec_sync);
  randomMV(map_sync, size);

  // вывод Map и Vector
  cout << "Исходные контейнеры:" << endl;
  coutMV(vec_sync, map_sync);

  //удаление не более 15 значений у Vector
  vec_sync.erase(vec_sync.end() - dist(r), vec_sync.end());

  //удаление не более 15 значений у Map
  auto v_beg_del = map_sync.end();
  advance(v_beg_del, - dist(r));
  map_sync.erase(v_beg_del, map_sync.end());

  cout << "После удаления:" << endl;
  coutMV(vec_sync, map_sync);

  // синхронизация Vector
  auto m_beg_del = remove_if(vec_sync.begin(), vec_sync.end(), [&map_sync](int& v) {
    for (auto a : map_sync)
      if (a.second == v)
        return false;
    return true;
  });

  vec_sync.erase(m_beg_del, vec_sync.end());

  // синхронизация Map
  for (auto it = map_sync.begin(); it != map_sync.end();)
    if (vec_sync.end() == find(vec_sync.begin(), vec_sync.end(), it->second))
      it = map_sync.erase(it);
    else
      ++it;

  cout << "Синхронизация:" << endl;
  coutMV(vec_sync, map_sync);

  return 0;
}


