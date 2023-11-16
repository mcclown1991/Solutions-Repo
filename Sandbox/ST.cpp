#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <set>
#include <stdio.h>

namespace Optimized_shortest {
	size_t CalculateHash(int x, int y) {
		return (x * 100000) + y;
	}

	std::pair<int, int> CalculateHashed(int hash) {
		return { hash / 100000, hash % 100000 };
	}

	class Solution {
	public:
		struct PathInfo {
			PathInfo() {}
			PathInfo(int x, int y, std::pair<int, int> const& portal) : x(x), y(y), cost(portal.second), weight(1), portal(portal.first), isPortalJump(false) {
				hash = CalculateHash(x, y);
			}
			bool operator==(PathInfo const& rhs) const {
				return x == rhs.x && y == rhs.y;
			}
			int x, y;
			int cost;
			int weight;
			int portal;
			bool isPortalJump;
			size_t hash;
		};
		using PATHHASH_VECTOR = std::vector<std::vector<size_t>>;
		using PATHINFO_VECTOR = std::vector<std::vector<PathInfo>>;
		using PORTAL_VECTOR = std::map<int, std::vector<std::pair<int, int>>>;

		struct Compare {
			Compare(PATHINFO_VECTOR& reference) : reference(reference) {}
			bool operator()(size_t left, size_t right) const {
				auto[lx, ly] = CalculateHashed(left);
				auto[rx, ry] = CalculateHashed(right);
				return reference[ly][lx].cost > reference[ry][rx].cost;
			}

		private:
			PATHINFO_VECTOR& reference;
		};


		int MahattanDistance(PathInfo const& node, PathInfo const& goal) const {
			return std::abs(goal.x - node.x) + std::abs(goal.y - node.y);
		}

		const PathInfo* FindNearestTeleporter(PathInfo const& node) {
			const PathInfo* nearest = nullptr;
			size_t distance{ 999 };
			for (auto const&[num, point] : portal) {
				for (auto const&[x, y] : point) {
					auto const& portalNode = mPathNodes[y][x];
					auto mhd = MahattanDistance(portalNode, node);
					if (mhd < distance) {
						distance = mhd;
						nearest = &portalNode;
					}
				}
			}

			return nearest;
		}

		int MahattanDistanceAdmissible(PathInfo const node, PathInfo const& goal) {
			auto h = MahattanDistance(node, goal);
			const auto* tx = FindNearestTeleporter(node);
			const auto* ty = FindNearestTeleporter(goal);
			return std::min(h, MahattanDistance(node, *tx) + MahattanDistance(goal, *ty));
		}

		int FindPath(std::vector<std::vector<std::pair<int, int>>>& path) {
			mPathNodes = PATHINFO_VECTOR(path.size());
			for (int i = 0; i < path.size(); ++i) {
				mPathNodes[i].reserve(path[i].size());
				for (int j = 0; j < path[i].size(); ++j) {
					mPathNodes[i].emplace_back(PathInfo(j, i, path[i][j]));
					if (path[i][j].first > 0) {
						if (portal.count(path[i][j].first) == 0) {
							portal.insert(std::pair(path[i][j].first, std::vector<std::pair<int, int>>()));
						}
						portal[path[i][j].first].emplace_back(std::pair(j, i));
					}
				}
			}

			PathInfo* found = nullptr;
			// goal (path.size() - 1, path[0].size() - 1)

			Compare compare(mPathNodes);
			std::priority_queue<size_t, std::vector<size_t>, Compare> priorityQueue(compare);
			std::unordered_set<size_t> visited;
			bool m_IsGoal = false; //for knowing if we reached goal state

			priorityQueue.push(0);
			mPathNodes[0][0].cost = 0;

			size_t goal = CalculateHash(path.size() - 1, path[0].size() - 1);


			while (!priorityQueue.empty() && !m_IsGoal) {

				auto hash = priorityQueue.top();
				priorityQueue.pop();

				if (!visited.count(hash))
					visited.insert(hash);

				// get current node
				auto[x, y] = CalculateHashed(hash);
				auto const& node = mPathNodes[y][x];

				auto ExpandNode = [this, &priorityQueue, &found, goal, &visited, &m_IsGoal](auto const& node, auto& neighbour, bool isPortal = false) mutable {
					if (m_IsGoal)
						return;

					if (neighbour.hash == goal) {
						mPreviousNodes.insert(std::pair(neighbour.hash, node.hash));
						found = &neighbour;
						m_IsGoal = true;
						return;
					}

					auto[gx, gy] = CalculateHashed(goal);
					auto const& goalNode = mPathNodes[gy][gx];
					auto g = node.cost + (isPortal ? 0 : 1);
					auto h = MahattanDistanceAdmissible(neighbour, goalNode);
					auto f = g + h;

					if (!visited.count(neighbour.hash) || f < neighbour.weight) {
						neighbour.cost = f;
						neighbour.weight = g;
						mPreviousNodes.insert(std::pair(neighbour.hash, node.hash));
						priorityQueue.push(neighbour.hash);
						neighbour.isPortalJump = isPortal;
					}
				};

				// expand right
				if (node.x < path[node.x].size() - 1)
					ExpandNode(node, mPathNodes[node.y][node.x + 1]);

				if (node.x > 0)
					ExpandNode(node, mPathNodes[node.y][node.x - 1]);

				if (node.y < path.size() - 1)
					ExpandNode(node, mPathNodes[node.y + 1][node.x]);

				if (node.y > 0)
					ExpandNode(node, mPathNodes[node.y - 1][node.x]);

				if (portal.count(node.portal)) {
					for (auto const& portalNode : portal[node.portal]) {
						ExpandNode(node, mPathNodes[portalNode.second][portalNode.first], true);
					}
				}


			}

			int result = path[found->y][found->x].second;

			std::stack<PathInfo*> pathFound;
			pathFound.push(found);
			while (found && found->hash != 0) {
				auto& nodeHash = mPreviousNodes[found->hash];
				auto[nx, ny] = CalculateHashed(nodeHash);
				auto& node = mPathNodes[ny][nx];
				found = &node;
				result += path[ny][nx].second;
				pathFound.push(&node);
			}

			while (!pathFound.empty()) {
				auto* node = pathFound.top();
				pathFound.pop();
				printf("%d,%d", node->x, node->y);
				if (CalculateHash(node->x, node->y) != goal)
					printf(" -> ");
			}

			return result;
		}

	protected:
		PORTAL_VECTOR portal;
		PATHINFO_VECTOR mPathNodes;
		std::unordered_map<size_t, size_t> mPreviousNodes;
	};
}

int main()
{
	int M, N, num, x{ 0 }, y{ 0 };
	std::vector<std::vector<std::pair<int, int>>> path;
	scanf("%d %d", &M, &N);
	path.emplace_back(std::vector<std::pair<int, int>>());
	bool reading{ true };
	while (!(M == x && N - 1 == y)) {
		if (M == x) {
			path.emplace_back(std::vector<std::pair<int, int>>());
			x = 0;
			++y;
		}
		scanf("%d", &num);
		++x;
		path[y].emplace_back(std::pair(num, 0));
	}
	x = y = 0;
	while(!(M == x && N - 1 == y)) {
		if (M == x) {
			x = 0;
			++y;
		}
		scanf("%d", &num);
		path[y][x++].second = num;
	}

	Optimized_shortest::Solution sol;
	int step = sol.FindPath(path);
	printf("\n%d", step);
	return 0;
}
