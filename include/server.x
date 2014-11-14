typedef string longstring<>;

enum RaftType { LogCabinType, GoRaftType };

program server_api {
  version api_v1 {
  	bool setup(RaftType) = 1;
  } = 1;
} = 0x40048086;

