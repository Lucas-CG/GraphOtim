#include <queue>
#include <utility>
#include <iostream>
#include <vector>

class CompareDist
{
public:
    bool operator()(std::pair<int,int> n1,std::pair<int,int> n2) {
        return n1.second < n2.second;
    }
};

int main(){

	std::priority_queue< std::pair<int, int>, std::vector< std::pair<int, int> >, CompareDist > pq;

	std::pair<int, int> pair1(1, 2);
	std::pair<int, int> pair2(3, 4);

	pq.emplace(pair1);
	pq.emplace(pair2);

	while(!pq.empty())
	{
		std::cout << " " << pq.top().second << std::endl;
		pq.pop();
	}

	return 0;
}
