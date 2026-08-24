/*******************************************************************************
 * File: mod
 * Project: InfiniteTag
 * Created: 2026/6/6
 * Author: eternalfuture-e38299
 * Github: https://github.com/eternalfuture-e38299
 *
 * TEFKernel-Cpp-Wrapper 重写版（C++ / RAII）。
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 *******************************************************************************/

#include "mod_core.h"
#include "mod_logger.h"
#include "patchlib/field.hpp"
#include "patchlib/method.hpp"

using TEFKernel::PatchLib::Field;
using TEFKernel::PatchLib::Method;
using TEFKernel::PatchLib::Type;
using TEFKernel::PatchLib::HookId;

// 字段包装对象
static Field g_tag;

// Hook ID
static HookId g_hook_id;

// Hook 函数
static void OnResetEffects(patch_handle_t instance, void** args, void* result,
                         const patch_method_signature_t* sig_info) {
    (void)args; (void)result; (void)sig_info;

    if (!instance) return;
    g_tag.SetValue(instance, 9999);
}

// 模块初始化
static void init_mod(kernel_mod_handle_t* handle) {
    (void)handle;
    mod_logger_write(MOD_LOG_LEVEL_INFO, "InfiniteTag", "Loading...");

    try {
        Type playerType("Terraria", "Player");

        g_tag = playerType.GetField("maxTagEffects");

        if (!g_tag) {
            mod_logger_write(MOD_LOG_LEVEL_ERROR, "InfiniteTag", "Can't get fields");
            return;
        }

        Method method = playerType.GetMethod("ResetEffects", 0);
        if (method) {
            g_hook_id = method.InstallPrePostHook(nullptr, OnResetEffects);
            if (g_hook_id.IsValid()) {
                mod_logger_write(MOD_LOG_LEVEL_INFO, "InfiniteTag", "Hook installed");
            }
        }
    } catch (const std::exception& e) {
        mod_logger_write(MOD_LOG_LEVEL_ERROR, "InfiniteTag", "Init failed: %s", e.what());
        return;
    }
}

// 模块清理
static void cleanup_mod(kernel_mod_handle_t* handle) {
    (void)handle;
    if (g_hook_id.IsValid()) {
        Method::UninstallHook(g_hook_id);
        g_hook_id.Reset();
    }
    mod_logger_write(MOD_LOG_LEVEL_INFO, "InfiniteTag", "Unloaded");
}

// 模块信息
static kernel_mod_info_t g_info = {
    "eternal.future.infiniteTag",
    20260606,
    1,
    "1.0.0"
};

static kernel_mod_info_t* get_info(void) {
    return &g_info;
}

// 操作函数表
static kernel_mod_ops_t g_ops = {
    init_mod,
    cleanup_mod,
    get_info
};

// 入口点
kernel_mod_ops_t* create_kernel_mod(void) {
    return &g_ops;
}