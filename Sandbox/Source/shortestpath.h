#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <set>
#include <iostream>

using namespace std;

namespace shortest_path {
	class Solution {
	public:
		
		struct pathInfo {
			pathInfo() {}
			pathInfo(int x, int y, int portal) : x(x), y(y), cost(INT_MAX), weight(1), portal(portal), isPortalJump(false), parent(nullptr) {}
			bool operator==(pathInfo const& rhs) const {
				return x == rhs.x && y == rhs.y;
			}
			int x, y;
			int cost;
			int weight;
			int portal;
			bool isPortalJump;
			pathInfo* parent;
		};

		std::vector<pathInfo*> mNodes;
		pathInfo* MakePathNode(int x, int y, int portal) {
			auto* node = new pathInfo(x, y, portal);
			mNodes.emplace_back(node);
			return node;
		}

		struct Hash {
			size_t operator()(pathInfo* node) const {
				return (1 + node->x) * 10000 + (1 + node->y);
			}
		};

		struct Equal {
			bool operator()(pathInfo* lhs, pathInfo* rhs) const {
				return lhs->x == rhs->x && lhs->y == rhs->y;
			}
		};

		struct Compare {
			bool operator()(pathInfo* left, pathInfo* right) const {
				return left->weight > right->weight;
			}
		};

		int mahattan(pathInfo const& node, pathInfo const& goal) {
			return std::abs(goal.x - node.x) + std::abs(goal.y - node.y);
		}


		
		int shortestpath(std::vector<std::vector<int>>& path) {
			std::map<int, std::vector<std::pair<int, int>>> portal;
			
			for(int i = 0; i < path.size(); ++i) {
				for(int j = 0; j < path[i].size(); ++j) {
					if(path[i][j] > 0) {
						if(portal.count(path[i][j]) == 0) {
							portal.insert(std::pair(path[i][j], std::vector<std::pair<int, int>>()));
						}
						portal[path[i][j]].emplace_back(std::pair(j, i));
					}
				}
			}
			pathInfo* solution = nullptr;
			// goal (path.size() - 1, path[0].size() - 1)
			
			std::priority_queue<pathInfo*, std::vector<pathInfo*>, Compare> pq;
			std::unordered_set<pathInfo*, Hash, Equal> visited;
			bool m_IsGoal = false; //for knowing if we reached goal state

			pq.push(MakePathNode(0, 0, path[0][0]));
			pathInfo goal(path.size() - 1, path[0].size() - 1, 0);
			

			while(!m_IsGoal) {
				if (pq.empty())
					return 0;
				
				auto node = pq.top();
				pq.pop();

				std::cout << "node: " << node->x << ", " << node->y << std::endl;

				if (!visited.count(node))
					visited.insert(node);

				// expand right
				if(node->x < path[node->x].size() - 1) {
					auto* right = MakePathNode(node->x + 1, node->y, path[node->y][node->x + 1]);
					if(!visited.count(right)) {
						right->weight = mahattan(*right, goal) + node->weight;
						right->cost = node->cost;
						right->parent = node;
						if(*right == goal) {
							m_IsGoal = true;
							solution = right;
							break;
						}
						pq.push(right);
					}
				}
				// expand left
				if (node->x > 0) {
					auto* left = MakePathNode(node->x - 1, node->y, path[node->y][node->x - 1]);
					// check if visited
					if (!visited.count(left)) {
						left->weight += mahattan(*left, goal) + node->weight;
						left->cost = node->cost;
						left->parent = node;
						if (*left == goal) {
							m_IsGoal = true;
							solution = left;
							break;
						}
						pq.push(left);
					}
				}
				//expand bottom
				if(node->y < path.size() - 1) {
					auto* bottom = MakePathNode(node->x, node->y + 1, path[node->y + 1][node->x]);
					if(!visited.count(bottom)) {
						bottom->weight += mahattan(*bottom, goal) + node->weight;
						bottom->cost = node->cost;
						bottom->parent = node;
						if (*bottom == goal) {
							m_IsGoal = true;
							solution = bottom;
							break;
						}
						pq.push(bottom);
					}
				}
				//expand top
				if(node->y > 0) {
					auto* top = MakePathNode(node->x, node->y - 1, path[node->y - 1][node->x]);
					if(!visited.count(top)) {
						top->weight += mahattan(*top, goal) + node->weight;
						top->cost = node->cost;
						top->parent = node;
						if (*top == goal) {
							m_IsGoal = true;
							solution = top;
							break;
						}
						pq.push(top);
					}

				}
				//check if node is portal
				if(portal.count(node->portal)) {
					// expand portal
					for(auto const& portalNode : portal[node->portal]) {
						auto* iter = MakePathNode(portalNode.first, portalNode.second, path[portalNode.second][portalNode.first]);
						iter->weight = 0;
						if(!visited.count(iter)) {
							iter->weight = mahattan(*iter, goal) + node->weight;
							iter->cost = node->cost;
							iter->parent = node;
							iter->isPortalJump = true;
							if(*iter == goal) {
								m_IsGoal = true;
								solution = iter;
							}
							pq.push(iter);
						}
					}
				}
			}

			int result = 0;
			while (solution->parent != nullptr) {
				auto node = solution->parent;
				if(!node->isPortalJump)
					++result;
				std::cout << node->x << "," << node->y << "->";
				solution = node;
			}

			for (auto* node : mNodes) {
				delete node;
			}

			return result;
		}
	};
}

namespace Optimized_shortest {
	size_t CalculateHash(int x, int y) {
		return (x * 100000) + y;
	}

	std::pair<int, int> CalculateHashed(int hash) {
		return { hash / 100000, hash % 100000 };
	}

	class Solution {
	public:
		using PATHHASH_VECTOR = std::vector<std::vector<size_t>>;
		
		struct pathInfo {
			pathInfo() {}
			pathInfo(int x, int y, int portal) : x(x), y(y), cost(INT_MAX), weight(1), portal(portal), isPortalJump(false) {
				hash = CalculateHash(x, y);
			}
			bool operator==(pathInfo const& rhs) const {
				return x == rhs.x && y == rhs.y;
			}
			int x, y;
			int cost;
			int weight;
			int portal;
			bool isPortalJump;
			size_t hash;
		};
		using PATHINFO_VECTOR = std::vector<std::vector<pathInfo>>;
		using PORTAL_VECTOR = std::map<int, std::vector<std::pair<int, int>>>;

		struct Compare {
			Compare(PATHINFO_VECTOR& reference) : reference(reference) {}
			bool operator()(size_t left, size_t right) const {
				auto [lx, ly] = CalculateHashed(left);
				auto [rx, ry] = CalculateHashed(right);
				return reference[ly][lx].cost > reference[ry][rx].cost;
			}

		private:
			PATHINFO_VECTOR& reference;
		};


		int mahattan(pathInfo const& node, pathInfo const& goal) {
			return std::abs(goal.x - node.x) + std::abs(goal.y - node.y);
		}

		const pathInfo* FindNearestTeleporter(pathInfo const& node) {
			const pathInfo* nearest = nullptr;
			size_t distance{INT_MAX};
			for (auto const& [num, point] : portal) {
				for (auto const& [x, y] : point) {
					auto const& portalNode = mPathNodes[y][x];
					auto mhd = mahattan(portalNode, node);
					if (mhd < distance) {
						distance = mhd;
						nearest = &portalNode;
					}
				}
			}

			return nearest;
		}

		int MahattanDistanceAdmissible(pathInfo const node, pathInfo const& goal) {
			auto h = mahattan(node, goal);
			const auto* tx = FindNearestTeleporter(node);
			const auto* ty = FindNearestTeleporter(goal);
			return std::min(h, mahattan(node, *tx) + mahattan(goal, *ty));
		}

		int shortestpath(std::vector<std::vector<int>>& path) {
			mPathNodes = PATHINFO_VECTOR(path.size());
			for (int i = 0; i < path.size(); ++i) {
				mPathNodes[i].reserve(path[i].size());
				for (int j = 0; j < path[i].size(); ++j) {
					mPathNodes[i].emplace_back(pathInfo(j, i, path[i][j]));
					if (path[i][j] > 0) {
						if (portal.count(path[i][j]) == 0) {
							portal.insert(std::pair(path[i][j], std::vector<std::pair<int, int>>()));
						}
						portal[path[i][j]].emplace_back(std::pair(j, i));
					}
				}
			}
			
			pathInfo* found = nullptr;
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
				auto [x, y] = CalculateHashed(hash);
				auto const& node = mPathNodes[y][x];

				std::cout << "node: " << node.x << ", " << node.y << std::endl;

				auto ExpandNode = [this, &priorityQueue, &found, goal, &visited, &m_IsGoal](auto const& node, auto& neighbour, bool isPortal = false) mutable {
					if (m_IsGoal)
						return;

					if (neighbour.hash == goal) {
						mPreviousNodes.insert(std::pair(neighbour.hash, node.hash));
						found = &neighbour;
						m_IsGoal = true;
						return;
					}

					auto [gx, gy] = CalculateHashed(goal);
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

			int result = 0;

			while (found && found->hash != 0) {
				auto& nodeHash = mPreviousNodes[found->hash];
				auto [nx, ny] = CalculateHashed(nodeHash);
				auto& node = mPathNodes[ny][nx];
				found = &node;
				if(!node.isPortalJump)
				++result;

				std::cout << node.x << "," << node.y << "->";
			}

			return result;
		}

		protected:
			PORTAL_VECTOR portal;
			PATHINFO_VECTOR mPathNodes;
			std::unordered_map<size_t, size_t> mPreviousNodes;
	};
}


//#include <winsock2.h>
//using namespace std;
//
//#pragma comment(lib,"ws2_32.lib") 
//#pragma warning(disable:4996) 
//
//#define SERVER "127.0.0.1"  // or "localhost" - ip address of UDP server
//#define BUFLEN 512  // max length of answer
//#define PORT 8888  // the port on which to listen for incoming data
//
//
//template <class... Args>
//class Slot {
//public:
//	template <class Provider, class Signature>
//Slot(Provider const& p, Signature s) {
//    call_back(class_interface<Provider>(p, s));
//	}
//
//void operator()(Args... args) {
//    call_back(args...);
//	}
//protected:
//	template <class... Args>
//	struct function_interface {
//	    virtual ~function_interface() {}
//	    virtual void operator()(Args... args) = 0;
//	};
//
//	template <class owner>
//	struct class_interface : function_interface<Args ...> {
//	    using method_type = void(owner::*)(Args...);
//		class_interface(owner const& owner, method_type method) : o(owner), method(method){
//			
//		}
//		virtual void operator()(Args... args) {
//	        (o.*method)(std::forward<Args>(args)...);
//		}
//
//	    owner const& o;
//	    method_type method;
//	};
//
//private:
//	function_interface<Args...> call_back;
//};
//
//
//
//template <class... Args>
//class Signal {
//
//    template<class Provider_, class Signal_ , class Receiver_, class Slot_>
//	void Connect(Provider_ const& p, Signal_ const& sig, Receiver_ const& r, Slot_ s) {
//	    //slots
//    }
//    std::map<size_t, Slot<Args...>> slots;
//};
//
//
//int main()
//{
//#ifdef CLIENT
//    system("title UDP Client");
//
//    // initialise winsock
//    WSADATA ws;
//    printf("Initialising Winsock...");
//    if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
//    {
//        printf("Failed. Error Code: %d", WSAGetLastError());
//        return 1;
//    }
//    printf("Initialised.\n");
//
//    // create socket
//    sockaddr_in server;
//    int client_socket;
//    if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) // <<< UDP socket
//    {
//        printf("socket() failed with error code: %d", WSAGetLastError());
//        return 2;
//    }
//
//    // setup address structure
//    memset((char*)&server, 0, sizeof(server));
//    server.sin_family = AF_INET;
//    server.sin_port = htons(PORT);
//    server.sin_addr.S_un.S_addr = inet_addr(SERVER);
//
//    // start communication
//    while (true)
//    {
//        char message[BUFLEN];
//        printf("Enter message: ");
//        cin.getline(message, BUFLEN);
//
//        // send the message
//        if (sendto(client_socket, message, strlen(message), 0, (sockaddr*)&server, sizeof(sockaddr_in)) == SOCKET_ERROR)
//        {
//            printf("sendto() failed with error code: %d", WSAGetLastError());
//            return 3;
//        }
//
//        // receive a reply and print it
//        // clear the answer by filling null, it might have previously received data
//        char answer[BUFLEN] = {};
//
//        // try to receive some data, this is a blocking call
//        int slen = sizeof(sockaddr_in);
//        int answer_length;
//        if (answer_length = recvfrom(client_socket, answer, BUFLEN, 0, (sockaddr*)&server, &slen) == SOCKET_ERROR)
//        {
//            printf("recvfrom() failed with error code: %d", WSAGetLastError());
//            exit(0);
//        }
//
//        cout << answer << "\n";
//    }
//
//    closesocket(client_socket);
//    WSACleanup();
//
////#endif
//
//    system("title UDP Server");
//
//    sockaddr_in server, client;
//
//    // initialise winsock
//    WSADATA wsa;
//    printf("Initialising Winsock...");
//    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
//    {
//        printf("Failed. Error Code: %d", WSAGetLastError());
//        exit(0);
//    }
//    printf("Initialised.\n");
//
//    // create a socket
//    SOCKET server_socket;
//    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
//    {
//        printf("Could not create socket: %d", WSAGetLastError());
//    }
//    printf("Socket created.\n");
//
//    // prepare the sockaddr_in structure
//    server.sin_family = AF_INET;
//    server.sin_addr.s_addr = INADDR_ANY;
//    server.sin_port = htons(PORT);
//
//    // bind
//    if (bind(server_socket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
//    {
//        printf("Bind failed with error code: %d", WSAGetLastError());
//        exit(EXIT_FAILURE);
//    }
//    puts("Bind done.");
//
//    while (true)
//    {
//        printf("Waiting for data...");
//        fflush(stdout);
//        char message[BUFLEN] = {};
//
//        // try to receive some data, this is a blocking call
//        int message_len;
//        int slen = sizeof(sockaddr_in);
//        if (message_len = recvfrom(server_socket, message, BUFLEN, 0, (sockaddr*)&client, &slen) == SOCKET_ERROR)
//        {
//            printf("recvfrom() failed with error code: %d", WSAGetLastError());
//            exit(0);
//        }
//
//        // print details of the client/peer and the data received
//        printf("Received packet from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
//        printf("Data: %s\n", message);
//
//        cin.getline(message, BUFLEN);
//
//        // reply the client with 2the same data
//        if (sendto(server_socket, message, strlen(message), 0, (sockaddr*)&client, sizeof(sockaddr_in)) == SOCKET_ERROR)
//        {
//            printf("sendto() failed with error code: %d", WSAGetLastError());
//            return 3;
//        }
//    }
//
//    closesocket(server_socket);
//    WSACleanup();
//
//#endif
//
//    float original{ -15.256f };
//    unsigned int* cheat = reinterpret_cast<unsigned int*>(&original);
//
//    char buffer[4];
//    unsigned int value = *cheat;
//	
//    buffer[0] = (value >> 24) & 0xFF;
//    buffer[1] = (value >> 16) & 0xFF;
//    buffer[2] = (value >> 8) & 0xFF;
//    buffer[3] = value & 0xFF;
//
//    unsigned int i = (buffer[0] << 24) | ((buffer[1] & 0xFF) << 16) | ((buffer[2] & 0xFF) << 8) | (buffer[3] & 0xFF);
//    float* destination = reinterpret_cast<float*>(&i);
//    float final = *destination;
//}