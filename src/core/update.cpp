#include "update.h"

Update::Update(update_type_t type, vertex_id_t src, vertex_id_t dst)
    : type{type}, src{src}, dst{dst} {}
