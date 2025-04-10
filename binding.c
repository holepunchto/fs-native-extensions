#include <node_api.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

#include "include/fs-ext.h"
#include "macros.h"

typedef struct {
  fs_ext_lock_t req;

  napi_env env;
  napi_ref ctx;
  napi_ref cb;
} fs_ext_napi_lock_t;

typedef struct {
  fs_ext_trim_t req;

  napi_env env;
  napi_ref ctx;
  napi_ref cb;
} fs_ext_napi_trim_t;

typedef struct {
  fs_ext_sparse_t req;

  napi_env env;
  napi_ref ctx;
  napi_ref cb;
} fs_ext_napi_sparse_t;

typedef struct {
  fs_ext_swap_t req;

  napi_env env;
  napi_ref ctx;
  napi_ref cb;
} fs_ext_napi_swap_t;

typedef struct {
  fs_ext_get_attr_t req;

  napi_env env;
  napi_ref ctx;
  napi_ref cb;
} fs_ext_napi_get_attr_t;

typedef struct {
  fs_ext_set_attr_t req;

  napi_env env;
  napi_ref ctx;
  napi_ref cb;
} fs_ext_napi_set_attr_t;

typedef struct {
  fs_ext_remove_attr_t req;

  napi_env env;
  napi_ref ctx;
  napi_ref cb;
} fs_ext_napi_remove_attr_t;

typedef struct {
  fs_ext_list_attrs_t req;

  napi_env env;
  napi_ref ctx;
  napi_ref cb;
} fs_ext_napi_list_attrs_t;

static void
on_fs_ext_lock(fs_ext_lock_t *req, int status) {
  fs_ext_napi_lock_t *r = (fs_ext_napi_lock_t *) req;

  napi_env env = r->env;

  napi_handle_scope scope;
  napi_open_handle_scope(env, &scope);

  napi_value argv[1];

  napi_value ctx;
  napi_get_reference_value(env, r->ctx, &ctx);

  napi_value callback;
  napi_get_reference_value(env, r->cb, &callback);

  if (req->result < 0) {
    napi_value code;
    napi_create_string_utf8(env, uv_err_name(req->result), -1, &code);

    napi_value message;
    napi_create_string_utf8(env, uv_strerror(req->result), -1, &message);

    napi_create_error(env, code, message, &argv[0]);
  } else {
    napi_get_null(env, &argv[0]);
  }

  NAPI_MAKE_CALLBACK(env, NULL, ctx, callback, 1, argv, NULL);

  napi_close_handle_scope(env, scope);

  napi_delete_reference(env, r->cb);
  napi_delete_reference(env, r->ctx);
}

static void
on_fs_ext_trim(fs_ext_trim_t *req, int status) {
  fs_ext_napi_trim_t *r = (fs_ext_napi_trim_t *) req;

  napi_env env = r->env;

  napi_handle_scope scope;
  napi_open_handle_scope(env, &scope);

  napi_value argv[1];

  napi_value ctx;
  napi_get_reference_value(env, r->ctx, &ctx);

  napi_value callback;
  napi_get_reference_value(env, r->cb, &callback);

  if (req->result < 0) {
    napi_value code;
    napi_create_string_utf8(env, uv_err_name(req->result), -1, &code);

    napi_value message;
    napi_create_string_utf8(env, uv_strerror(req->result), -1, &message);

    napi_create_error(env, code, message, &argv[0]);
  } else {
    napi_get_null(env, &argv[0]);
  }

  NAPI_MAKE_CALLBACK(env, NULL, ctx, callback, 1, argv, NULL);

  napi_close_handle_scope(env, scope);

  napi_delete_reference(env, r->cb);
  napi_delete_reference(env, r->ctx);
}

static void
on_fs_ext_sparse(fs_ext_sparse_t *req, int status) {
  fs_ext_napi_sparse_t *r = (fs_ext_napi_sparse_t *) req;

  napi_env env = r->env;

  napi_handle_scope scope;
  napi_open_handle_scope(env, &scope);

  napi_value argv[1];

  napi_value ctx;
  napi_get_reference_value(env, r->ctx, &ctx);

  napi_value callback;
  napi_get_reference_value(env, r->cb, &callback);

  if (req->result < 0) {
    napi_value code;
    napi_create_string_utf8(env, uv_err_name(req->result), -1, &code);

    napi_value message;
    napi_create_string_utf8(env, uv_strerror(req->result), -1, &message);

    napi_create_error(env, code, message, &argv[0]);
  } else {
    napi_get_null(env, &argv[0]);
  }

  NAPI_MAKE_CALLBACK(env, NULL, ctx, callback, 1, argv, NULL);

  napi_close_handle_scope(env, scope);

  napi_delete_reference(env, r->cb);
  napi_delete_reference(env, r->ctx);
}

static void
on_fs_ext_swap(fs_ext_swap_t *req, int status) {
  fs_ext_napi_swap_t *r = (fs_ext_napi_swap_t *) req;

  napi_env env = r->env;

  napi_handle_scope scope;
  napi_open_handle_scope(env, &scope);

  napi_value argv[1];

  napi_value ctx;
  napi_get_reference_value(env, r->ctx, &ctx);

  napi_value callback;
  napi_get_reference_value(env, r->cb, &callback);

  if (req->result < 0) {
    napi_value code;
    napi_create_string_utf8(env, uv_err_name(req->result), -1, &code);

    napi_value message;
    napi_create_string_utf8(env, uv_strerror(req->result), -1, &message);

    napi_create_error(env, code, message, &argv[0]);
  } else {
    napi_get_null(env, &argv[0]);
  }

  NAPI_MAKE_CALLBACK(env, NULL, ctx, callback, 1, argv, NULL);

  napi_close_handle_scope(env, scope);

  free((char *) req->from);
  free((char *) req->to);

  napi_delete_reference(env, r->cb);
  napi_delete_reference(env, r->ctx);
}

static void
on_fs_ext_get_attr(fs_ext_get_attr_t *req, int status, const uv_buf_t *value) {
  fs_ext_napi_get_attr_t *r = (fs_ext_napi_get_attr_t *) req;

  napi_env env = r->env;

  napi_handle_scope scope;
  napi_open_handle_scope(env, &scope);

  napi_value argv[2];

  napi_value ctx;
  napi_get_reference_value(env, r->ctx, &ctx);

  napi_value callback;
  napi_get_reference_value(env, r->cb, &callback);

  if (req->result < 0 && req->result != UV_ENODATA) {
    napi_value code;
    napi_create_string_utf8(env, uv_err_name(req->result), -1, &code);

    napi_value message;
    napi_create_string_utf8(env, uv_strerror(req->result), -1, &message);

    napi_create_error(env, code, message, &argv[0]);

    napi_get_null(env, &argv[1]);
  } else {
    napi_get_null(env, &argv[0]);

    if (req->result == UV_ENODATA) {
      napi_get_null(env, &argv[1]);
    } else {
      void *data;
      napi_create_arraybuffer(env, value->len, &data, &argv[1]);

      memcpy(data, value->base, value->len);
    }
  }

  NAPI_MAKE_CALLBACK(env, NULL, ctx, callback, 2, argv, NULL);

  napi_close_handle_scope(env, scope);

  free((char *) req->name);

  napi_delete_reference(env, r->cb);
  napi_delete_reference(env, r->ctx);
}

static void
on_fs_ext_set_attr(fs_ext_set_attr_t *req, int status) {
  fs_ext_napi_set_attr_t *r = (fs_ext_napi_set_attr_t *) req;

  napi_env env = r->env;

  napi_handle_scope scope;
  napi_open_handle_scope(env, &scope);

  napi_value argv[1];

  napi_value ctx;
  napi_get_reference_value(env, r->ctx, &ctx);

  napi_value callback;
  napi_get_reference_value(env, r->cb, &callback);

  if (req->result < 0) {
    napi_value code;
    napi_create_string_utf8(env, uv_err_name(req->result), -1, &code);

    napi_value message;
    napi_create_string_utf8(env, uv_strerror(req->result), -1, &message);

    napi_create_error(env, code, message, &argv[0]);
  } else {
    napi_get_null(env, &argv[0]);
  }

  NAPI_MAKE_CALLBACK(env, NULL, ctx, callback, 1, argv, NULL);

  napi_close_handle_scope(env, scope);

  free((char *) req->name);

  napi_delete_reference(env, r->cb);
  napi_delete_reference(env, r->ctx);
}

static void
on_fs_ext_remove_attr(fs_ext_remove_attr_t *req, int status) {
  fs_ext_napi_remove_attr_t *r = (fs_ext_napi_remove_attr_t *) req;

  napi_env env = r->env;

  napi_handle_scope scope;
  napi_open_handle_scope(env, &scope);

  napi_value argv[1];

  napi_value ctx;
  napi_get_reference_value(env, r->ctx, &ctx);

  napi_value callback;
  napi_get_reference_value(env, r->cb, &callback);

  if (req->result < 0 && req->result != UV_ENODATA) {
    napi_value code;
    napi_create_string_utf8(env, uv_err_name(req->result), -1, &code);

    napi_value message;
    napi_create_string_utf8(env, uv_strerror(req->result), -1, &message);

    napi_create_error(env, code, message, &argv[0]);
  } else {
    napi_get_null(env, &argv[0]);
  }

  NAPI_MAKE_CALLBACK(env, NULL, ctx, callback, 1, argv, NULL);

  napi_close_handle_scope(env, scope);

  free((char *) req->name);

  napi_delete_reference(env, r->cb);
  napi_delete_reference(env, r->ctx);
}

static void
on_fs_ext_list_attrs(fs_ext_list_attrs_t *req, int status, const char *attrs[], ssize_t len) {
  fs_ext_napi_list_attrs_t *r = (fs_ext_napi_list_attrs_t *) req;

  napi_env env = r->env;

  napi_handle_scope scope;
  napi_open_handle_scope(env, &scope);

  napi_value argv[2];

  napi_value ctx;
  napi_get_reference_value(env, r->ctx, &ctx);

  napi_value callback;
  napi_get_reference_value(env, r->cb, &callback);

  if (req->result < 0) {
    napi_value code;
    napi_create_string_utf8(env, uv_err_name(req->result), -1, &code);

    napi_value message;
    napi_create_string_utf8(env, uv_strerror(req->result), -1, &message);

    napi_create_error(env, code, message, &argv[0]);

    napi_get_null(env, &argv[1]);
  } else {
    napi_get_null(env, &argv[0]);

    napi_create_array_with_length(env, len, &argv[1]);

    for (size_t i = 0; i < len; i++) {
      napi_value value;
      napi_create_string_utf8(env, attrs[i], -1, &value);

      napi_set_element(env, argv[1], i, value);
    }
  }

  NAPI_MAKE_CALLBACK(env, NULL, ctx, callback, 2, argv, NULL);

  napi_close_handle_scope(env, scope);

  napi_delete_reference(env, r->cb);
  napi_delete_reference(env, r->ctx);
}

NAPI_METHOD(fs_ext_napi_try_lock) {
  NAPI_ARGV(4)
  NAPI_ARGV_UINT32(fd, 0)
  NAPI_ARGV_UINT32(offset, 1)
  NAPI_ARGV_UINT32(len, 2)
  NAPI_ARGV_UINT32(exclusive, 3)

  int err = fs_ext_try_lock(
    uv_get_osfhandle(fd),
    offset,
    len,
    exclusive ? FS_EXT_WRLOCK : FS_EXT_RDLOCK
  );

  if (err < 0) {
    napi_throw_error(env, uv_err_name(err), uv_strerror(err));
  }

  return NULL;
}

NAPI_METHOD(fs_ext_napi_wait_for_lock) {
  NAPI_ARGV(7)
  NAPI_ARGV_BUFFER_CAST(fs_ext_napi_lock_t *, req, 0)
  NAPI_ARGV_UINT32(fd, 1)
  NAPI_ARGV_UINT32(offset, 2)
  NAPI_ARGV_UINT32(length, 3)
  NAPI_ARGV_UINT32(exclusive, 4)

  req->env = env;

  napi_create_reference(env, argv[5], 1, &(req->ctx));
  napi_create_reference(env, argv[6], 1, &(req->cb));

  uv_loop_t *loop;
  napi_get_uv_event_loop(env, &loop);

  int err = fs_ext_wait_for_lock(
    loop,
    (fs_ext_lock_t *) req,
    uv_get_osfhandle(fd),
    offset,
    length,
    exclusive ? FS_EXT_WRLOCK : FS_EXT_RDLOCK,
    on_fs_ext_lock
  );

  if (err < 0) {
    napi_throw_error(env, uv_err_name(err), uv_strerror(err));
  }

  return NULL;
}

NAPI_METHOD(fs_ext_napi_try_downgrade_lock) {
  NAPI_ARGV(3)
  NAPI_ARGV_UINT32(fd, 0)
  NAPI_ARGV_UINT32(offset, 1)
  NAPI_ARGV_UINT32(len, 2)

  int err = fs_ext_try_downgrade_lock(uv_get_osfhandle(fd), offset, len);

  if (err < 0) {
    napi_throw_error(env, uv_err_name(err), uv_strerror(err));
  }

  return NULL;
}

NAPI_METHOD(fs_ext_napi_wait_for_downgrade_lock) {
  NAPI_ARGV(6)
  NAPI_ARGV_BUFFER_CAST(fs_ext_napi_lock_t *, req, 0)
  NAPI_ARGV_UINT32(fd, 1)
  NAPI_ARGV_UINT32(offset, 2)
  NAPI_ARGV_UINT32(length, 3)

  req->env = env;

  napi_create_reference(env, argv[4], 1, &(req->ctx));
  napi_create_reference(env, argv[5], 1, &(req->cb));

  uv_loop_t *loop;
  napi_get_uv_event_loop(env, &loop);

  int err = fs_ext_wait_for_downgrade_lock(
    loop,
    (fs_ext_lock_t *) req,
    uv_get_osfhandle(fd),
    offset,
    length,
    on_fs_ext_lock
  );

  if (err < 0) {
    napi_throw_error(env, uv_err_name(err), uv_strerror(err));
  }

  return NULL;
}

NAPI_METHOD(fs_ext_napi_try_upgrade_lock) {
  NAPI_ARGV(3)
  NAPI_ARGV_UINT32(fd, 0)
  NAPI_ARGV_UINT32(offset, 1)
  NAPI_ARGV_UINT32(len, 2)

  int err = fs_ext_try_upgrade_lock(uv_get_osfhandle(fd), offset, len);

  if (err < 0) {
    napi_throw_error(env, uv_err_name(err), uv_strerror(err));
  }

  return NULL;
}

NAPI_METHOD(fs_ext_napi_wait_for_upgrade_lock) {
  NAPI_ARGV(6)
  NAPI_ARGV_BUFFER_CAST(fs_ext_napi_lock_t *, req, 0)
  NAPI_ARGV_UINT32(fd, 1)
  NAPI_ARGV_UINT32(offset, 2)
  NAPI_ARGV_UINT32(length, 3)

  req->env = env;

  napi_create_reference(env, argv[4], 1, &(req->ctx));
  napi_create_reference(env, argv[5], 1, &(req->cb));

  uv_loop_t *loop;
  napi_get_uv_event_loop(env, &loop);

  int err = fs_ext_wait_for_upgrade_lock(
    loop,
    (fs_ext_lock_t *) req,
    uv_get_osfhandle(fd),
    offset,
    length,
    on_fs_ext_lock
  );

  if (err < 0) {
    napi_throw_error(env, uv_err_name(err), uv_strerror(err));
  }

  return NULL;
}

NAPI_METHOD(fs_ext_napi_unlock) {
  NAPI_ARGV(3)
  NAPI_ARGV_UINT32(fd, 0)
  NAPI_ARGV_UINT32(offset, 1)
  NAPI_ARGV_UINT32(len, 2)

  int err = fs_ext_unlock(uv_get_osfhandle(fd), offset, len);

  if (err < 0) {
    napi_throw_error(env, uv_err_name(err), uv_strerror(err));
  }

  return NULL;
}

NAPI_METHOD(fs_ext_napi_trim) {
  NAPI_ARGV(6)
  NAPI_ARGV_BUFFER_CAST(fs_ext_napi_trim_t *, req, 0)
  NAPI_ARGV_UINT32(fd, 1)
  NAPI_ARGV_UINT32(offset, 2)
  NAPI_ARGV_UINT32(length, 3)

  req->env = env;

  napi_create_reference(env, argv[4], 1, &(req->ctx));
  napi_create_reference(env, argv[5], 1, &(req->cb));

  uv_loop_t *loop;
  napi_get_uv_event_loop(env, &loop);

  int err = fs_ext_trim(
    loop,
    (fs_ext_trim_t *) req,
    uv_get_osfhandle(fd),
    offset,
    length,
    on_fs_ext_trim
  );

  if (err < 0) {
    napi_throw_error(env, uv_err_name(err), uv_strerror(err));
  }

  return NULL;
}

NAPI_METHOD(fs_ext_napi_sparse) {
  NAPI_ARGV(4)
  NAPI_ARGV_BUFFER_CAST(fs_ext_napi_sparse_t *, req, 0)
  NAPI_ARGV_UINT32(fd, 1)

  req->env = env;

  napi_create_reference(env, argv[2], 1, &(req->ctx));
  napi_create_reference(env, argv[3], 1, &(req->cb));

  uv_loop_t *loop;
  napi_get_uv_event_loop(env, &loop);

  int err = fs_ext_sparse(
    loop,
    (fs_ext_sparse_t *) req,
    uv_get_osfhandle(fd),
    on_fs_ext_sparse
  );

  if (err < 0) {
    napi_throw_error(env, uv_err_name(err), uv_strerror(err));
  }

  return NULL;
}

NAPI_METHOD(fs_ext_napi_swap) {
  NAPI_ARGV(5)
  NAPI_ARGV_BUFFER_CAST(fs_ext_napi_swap_t *, req, 0)
  NAPI_ARGV_UTF8_MALLOC(from_path, 1)
  NAPI_ARGV_UTF8_MALLOC(to_path, 2)

  req->env = env;

  napi_create_reference(env, argv[3], 1, &(req->ctx));
  napi_create_reference(env, argv[4], 1, &(req->cb));

  uv_loop_t *loop;
  napi_get_uv_event_loop(env, &loop);

  int err = fs_ext_swap(
    loop,
    (fs_ext_swap_t *) req,
    from_path,
    to_path,
    on_fs_ext_swap
  );

  if (err < 0) {
    napi_throw_error(env, uv_err_name(err), uv_strerror(err));
  }

  return NULL;
}

NAPI_METHOD(fs_ext_napi_get_attr) {
  NAPI_ARGV(5)
  NAPI_ARGV_BUFFER_CAST(fs_ext_napi_get_attr_t *, req, 0)
  NAPI_ARGV_UINT32(fd, 1)
  NAPI_ARGV_UTF8_MALLOC(name, 2)

  req->env = env;

  napi_create_reference(env, argv[3], 1, &(req->ctx));
  napi_create_reference(env, argv[4], 1, &(req->cb));

  uv_loop_t *loop;
  napi_get_uv_event_loop(env, &loop);

  int err = fs_ext_get_attr(
    loop,
    (fs_ext_get_attr_t *) req,
    uv_get_osfhandle(fd),
    name,
    on_fs_ext_get_attr
  );

  if (err < 0) {
    napi_throw_error(env, uv_err_name(err), uv_strerror(err));
  }

  return NULL;
}

NAPI_METHOD(fs_ext_napi_set_attr) {
  NAPI_ARGV(6)
  NAPI_ARGV_BUFFER_CAST(fs_ext_napi_set_attr_t *, req, 0)
  NAPI_ARGV_UINT32(fd, 1)
  NAPI_ARGV_UTF8_MALLOC(name, 2)
  NAPI_ARGV_BUFFER(value, 3)

  req->env = env;

  napi_create_reference(env, argv[4], 1, &(req->ctx));
  napi_create_reference(env, argv[5], 1, &(req->cb));

  uv_loop_t *loop;
  napi_get_uv_event_loop(env, &loop);

  uv_buf_t buf = uv_buf_init(value, value_len);

  int err = fs_ext_set_attr(
    loop,
    (fs_ext_set_attr_t *) req,
    uv_get_osfhandle(fd),
    name,
    &buf,
    on_fs_ext_set_attr
  );

  if (err < 0) {
    napi_throw_error(env, uv_err_name(err), uv_strerror(err));
  }

  return NULL;
}

NAPI_METHOD(fs_ext_napi_remove_attr) {
  NAPI_ARGV(5)
  NAPI_ARGV_BUFFER_CAST(fs_ext_napi_remove_attr_t *, req, 0)
  NAPI_ARGV_UINT32(fd, 1)
  NAPI_ARGV_UTF8_MALLOC(name, 2)

  req->env = env;

  napi_create_reference(env, argv[3], 1, &(req->ctx));
  napi_create_reference(env, argv[4], 1, &(req->cb));

  uv_loop_t *loop;
  napi_get_uv_event_loop(env, &loop);

  int err = fs_ext_remove_attr(
    loop,
    (fs_ext_remove_attr_t *) req,
    uv_get_osfhandle(fd),
    name,
    on_fs_ext_remove_attr
  );

  if (err < 0) {
    napi_throw_error(env, uv_err_name(err), uv_strerror(err));
  }

  return NULL;
}

NAPI_METHOD(fs_ext_napi_list_attrs) {
  NAPI_ARGV(4)
  NAPI_ARGV_BUFFER_CAST(fs_ext_napi_list_attrs_t *, req, 0)
  NAPI_ARGV_UINT32(fd, 1)

  req->env = env;

  napi_create_reference(env, argv[2], 1, &(req->ctx));
  napi_create_reference(env, argv[3], 1, &(req->cb));

  uv_loop_t *loop;
  napi_get_uv_event_loop(env, &loop);

  int err = fs_ext_list_attrs(
    loop,
    (fs_ext_list_attrs_t *) req,
    uv_get_osfhandle(fd),
    on_fs_ext_list_attrs
  );

  if (err < 0) {
    napi_throw_error(env, uv_err_name(err), uv_strerror(err));
  }

  return NULL;
}

NAPI_INIT() {
  NAPI_EXPORT_SIZEOF(fs_ext_napi_lock_t)
  NAPI_EXPORT_SIZEOF(fs_ext_napi_trim_t)
  NAPI_EXPORT_SIZEOF(fs_ext_napi_sparse_t)
  NAPI_EXPORT_SIZEOF(fs_ext_napi_swap_t)
  NAPI_EXPORT_SIZEOF(fs_ext_napi_get_attr_t)
  NAPI_EXPORT_SIZEOF(fs_ext_napi_set_attr_t)
  NAPI_EXPORT_SIZEOF(fs_ext_napi_remove_attr_t)
  NAPI_EXPORT_SIZEOF(fs_ext_napi_list_attrs_t)

  NAPI_EXPORT_FUNCTION(fs_ext_napi_try_lock)
  NAPI_EXPORT_FUNCTION(fs_ext_napi_wait_for_lock)
  NAPI_EXPORT_FUNCTION(fs_ext_napi_try_downgrade_lock)
  NAPI_EXPORT_FUNCTION(fs_ext_napi_wait_for_downgrade_lock)
  NAPI_EXPORT_FUNCTION(fs_ext_napi_try_upgrade_lock)
  NAPI_EXPORT_FUNCTION(fs_ext_napi_wait_for_upgrade_lock)
  NAPI_EXPORT_FUNCTION(fs_ext_napi_unlock)
  NAPI_EXPORT_FUNCTION(fs_ext_napi_trim)
  NAPI_EXPORT_FUNCTION(fs_ext_napi_sparse)
  NAPI_EXPORT_FUNCTION(fs_ext_napi_swap)
  NAPI_EXPORT_FUNCTION(fs_ext_napi_get_attr)
  NAPI_EXPORT_FUNCTION(fs_ext_napi_set_attr)
  NAPI_EXPORT_FUNCTION(fs_ext_napi_remove_attr)
  NAPI_EXPORT_FUNCTION(fs_ext_napi_list_attrs)
}
