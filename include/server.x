typedef string longstring<>;

enum RaftType { LogCabinType, EtcdType };

typedef string IPAddr<15>;

struct IPList {
	IPAddr nodes<>;
};

struct ClusterDesc {
	RaftType type;
	IPList public_ips;
    IPList private_ips;
	int nodeId;
};

struct Partition {
	int group1<>;
	int group2<>;
};

struct Part {
	IPAddr group1<>;
	IPAddr group2<>;
};

program server_api {
  version api_v1 {
        bool install(ClusterDesc) = 1;
        bool run(ClusterDesc) = 2;
	bool makePartition(Partition) = 3;
        bool healPartition(int) = 4;
        bool slowNetwork(bool) = 5;
  } = 1;
} = 0x40048086;
