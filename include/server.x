typedef string longstring<>;

enum RaftType { LogCabinType, GoRaftType };

struct Partition {
	int group1<>;
	int group2<>;
}

program server_api {
  version api_v1 {
	bool setup(RaftType) = 1;
	bool makePartition(Partition) = 2;
	bool healPartition() = 3;
  } = 1;
} = 0x40048086;

