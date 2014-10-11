
typedef string longstring<>;

struct kvpair {
	string key<512>;
	string val<>;
};

enum ServerError {
    /*
     * Set, Get or Remove failed because they key was not found.
     */
    KEY_NOT_FOUND,
    /*
     * Create operation failed because the key has no parent.
     */
    NO_PARENT,
    /*
     * The key has children so it cannot be deleted.
     */
    HAS_CHILDREN,
    /*
     * The key path violates our formatting rules
     */
    MALFORMED_KEY
};

// Encodes a boolean or an error code.
union MaybeBool switch (unsigned discriminant) {
 case 0:
     bool result;
 case 1:
     ServerError errorCode;
 default:
     void;
};

// Encodes a string or an error code.
union MabyeString switch (unsigned discriminant) {
 case 0:
     longstring result;
 case 1:
     ServerError errorCode;
 default:
     void;
};

// Encodes a set of strings or an error code.
union MaybeSetString switch (unsigned discriminant) {
 case 0:
     longstring result<>;
 case 1:
     ServerError errorCode;
 default:
     void;
};

program server_api {
  version api_v1 {
	MaybeBool create(kvpair) = 1;
	MaybeBool remove(longstring) = 2;
	MaybeBool set(kvpair) = 3;
        MaybeString get(longstring) = 4;
        MavbeSetString list(longstring) = 5;
  } = 1;
} = 0x40048086;

