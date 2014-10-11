
typedef string longstring<>;

#define CODE_SUCCESS 0
#define CODE_ERROR 1

struct kvpair {
	string key<512>;
	string val<>;
};

enum ErrorCode {
    /*
     * Set, Get or Remove failed because they key was not found.
     */
    KEY_NOT_FOUND_ERROR,
    /*
     * Create operation failed because the key has no parent.
     */
    NO_PARENT_ERROR,
    /*
     * The key has children so it cannot be deleted.
     */
    HAS_CHILDREN_ERROR,
    /*
     * The key path violates our formatting rules
     */
    MALFORMED_KEY_ERROR
};

// Encodes a boolean or an error code.
union MaybeBool switch (unsigned discriminant) {
 case CODE_SUCCESS:
     bool value;
 case CODE_ERROR:
     ErrorCode errorCode;
 default:
     void;
};

// Encodes a string or an error code.
union MaybeString switch (unsigned discriminant) {
 case CODE_SUCCESS:
     longstring value;
 case CODE_ERROR:
     ErrorCode errorCode;
 default:
     void;
};

// Encodes a set of strings or an error code.
union MaybeSetString switch (unsigned discriminant) {
 case CODE_SUCCESS:
     longstring value<>;
 case CODE_ERROR:
     ErrorCode errorCode;
 default:
     void;
};

program server_api {
  version api_v1 {
	MaybeBool create(kvpair) = 1;
	MaybeBool remove(longstring) = 2;
	MaybeBool set(kvpair) = 3;
        MaybeString get(longstring) = 4;
        MaybeSetString list(longstring) = 5;
  } = 1;
} = 0x40048086;

