static inline NSString *vm_prot_to_string(vm_prot_t p) {
    NSMutableArray *arr = [NSMutableArray array];
    if (p & VM_PROT_READ)   [arr addObject:@"R"];
    if (p & VM_PROT_WRITE)  [arr addObject:@"W"];
    if (p & VM_PROT_EXECUTE)[arr addObject:@"X"];
    if (p & VM_PROT_COPY)   [arr addObject:@"C"]; // copy-on-write
    if (arr.count == 0)     [arr addObject:@"-"];
    return [arr componentsJoinedByString:@""];
}

static inline NSString *inherit_to_string(vm_inherit_t v) {
    switch (v) {
        case VM_INHERIT_SHARE:    return @"SHARE";
        case VM_INHERIT_COPY:     return @"COPY";
        case VM_INHERIT_NONE:     return @"NONE";
        default:                  return [NSString stringWithFormat:@"UNKNOWN(%d)", v];
    }
}

static inline NSString *behavior_to_string(vm_behavior_t b) {
    switch (b) {
        case VM_BEHAVIOR_DEFAULT:    return @"DEFAULT";
        case VM_BEHAVIOR_RANDOM:     return @"RANDOM";
        case VM_BEHAVIOR_SEQUENTIAL: return @"SEQUENTIAL";
        case VM_BEHAVIOR_RSEQNTL:    return @"REVERSE_SEQ";
        case VM_BEHAVIOR_WILLNEED:   return @"WILLNEED";
        case VM_BEHAVIOR_DONTNEED:   return @"DONTNEED";
        default:                     return [NSString stringWithFormat:@"UNKNOWN(%d)", b];
    }
}

NSMutableString * PrintVMRegionBasicInfo64(vm_region_basic_info_data_64_t info) {
    NSMutableString * result = [NSMutableString new];
    [result appendString:@"vm_region_basic_info_64 {\n"];
    [result appendFormat:@"  protection       = %@ (0x%x)\n", vm_prot_to_string(info.protection), info.protection];
    [result appendFormat:@"  max_protection   = %@ (0x%x)\n", vm_prot_to_string(info.max_protection), info.max_protection];
    [result appendFormat:@"  inheritance   = %@ (%d)\n", inherit_to_string(info.inheritance), info.inheritance];
    [result appendFormat:@"  shared           = %u\n", info.shared];
    [result appendFormat:@"  reserved         = %u\n", info.reserved];
    [result appendFormat:@"  offset           = 0x%llx\n", info.offset];
    [result appendFormat:@"  behavior         = %@ (%d)\n", behavior_to_string(info.behavior), info.behavior];
    [result appendFormat:@"  user_wired_count = %hu\n", info.user_wired_count];
    [result appendString:@"}\n"];
    return result;
}

vm_region_basic_info_data_64_t detect_jb_payload(void) {
    mach_port_t object_name;
    mach_vm_size_t region_size = 0;
    mach_vm_address_t region_base = (uint64_t)vm_region_64;

    vm_region_basic_info_data_64_t info = {0};
    mach_msg_type_number_t info_cnt = VM_REGION_BASIC_INFO_COUNT_64;

    vm_region_64(mach_task_self(), (vm_address_t*)&region_base, (vm_size_t*)&region_size, VM_REGION_BASIC_INFO_64, (vm_region_info_t)&info, &info_cnt, &object_name);
    
    if(info.protection != VM_PROT_READ) {
        printf("jb payload injected!\n");
    }
    return info;
}

static inline NSString *vm_prot_to_str(vm_prot_t p) {
    NSMutableArray *a = [NSMutableArray array];
    if (p & VM_PROT_READ) [a addObject:@"R"];
    if (p & VM_PROT_WRITE) [a addObject:@"W"];
    if (p & VM_PROT_EXECUTE) [a addObject:@"X"];
    if (p & VM_PROT_COPY) [a addObject:@"C"];
    if (a.count == 0) [a addObject:@"-"];
    return [a componentsJoinedByString:@""];
}

static inline NSString *inherit_to_str(vm_inherit_t v) {
    switch (v) {
        case VM_INHERIT_SHARE: return @"SHARE";
        case VM_INHERIT_COPY:  return @"COPY";
        case VM_INHERIT_NONE:  return @"NONE";
        default:               return [NSString stringWithFormat:@"UNKNOWN(%d)", v];
    }
}

static inline NSString *share_mode_to_str(unsigned int s) {
    switch (s) {
        case SM_COW:         return @"COW";
        case SM_PRIVATE:     return @"PRIVATE";
        case SM_EMPTY:       return @"EMPTY";
        case SM_SHARED:      return @"SHARED";
        case SM_TRUESHARED:  return @"TRUE_SHARED";
        default:             return [NSString stringWithFormat:@"UNKNOWN(%u)", s];
    }
}

NSMutableString * PrintVMRegionRecurseInfo64(vm_region_submap_info_data_64_t info) {
    NSMutableString * result = [NSMutableString new];
    [result appendString:@"vm_region_submap_info_64 {\n"];
    [result appendFormat:@"  protection                 = %@ (0x%x)\n", vm_prot_to_string(info.protection), info.protection];
    [result appendFormat:@"  max_protection             = %@ (0x%x)\n", vm_prot_to_str(info.max_protection), info.max_protection];
    [result appendFormat:@"  inheritance                = %@ (%d)\n", inherit_to_str(info.inheritance), info.inheritance];
    [result appendFormat:@"  offset                     = 0x%llx\n", info.offset];
    [result appendFormat:@"  user_tag                   = %u\n", info.user_tag];
    [result appendFormat:@"  pages_resident             = %u\n", info.pages_resident];
    [result appendFormat:@"  pages_shared_now_private   = %u\n", info.pages_shared_now_private];
    [result appendFormat:@"  pages_swapped_out          = %u\n", info.pages_swapped_out];
    [result appendFormat:@"  pages_dirtied              = %u\n", info.pages_dirtied];
    [result appendFormat:@"  ref_count                  = %u\n", info.ref_count];
    [result appendFormat:@"  shadow_depth               = %u\n", info.shadow_depth];
    [result appendFormat:@"  external_pager             = %u\n", info.external_pager];
    [result appendFormat:@"  share_mode                 = %@ (%u)\n", share_mode_to_str(info.share_mode), info.share_mode];
    [result appendFormat:@"  is_submap                  = %u\n", info.is_submap];
    [result appendString:@"} "];
    return result;
}


void TraverseAndReadExecutableRegions(void) {
    vm_map_read_t task = mach_task_self();
    vm_address_t addr = MACH_VM_MIN_ADDRESS;
    vm_size_t size = 0;
    
    while (1) {
        natural_t depth = 0;
        vm_region_submap_info_data_64_t info;
        mach_msg_type_number_t count = VM_REGION_SUBMAP_INFO_COUNT_64;
        
        if (vm_region_recurse_64(task, &addr, &size, &depth, (vm_region_info_t)&info, &count)) {
            break;
        }
        
        struct mach_header_64 mh = {0};
        vm_size_t bytes_read = 0;
        kern_return_t kr = vm_read_overwrite(task, (mach_vm_address_t)addr, (mach_vm_size_t)sizeof(struct mach_header_64), (mach_vm_address_t)&mh, &bytes_read);
        if (kr == KERN_SUCCESS && bytes_read == sizeof(struct mach_header_64)) {
            if ((mh.magic == MH_MAGIC || mh.magic == MH_MAGIC_64) && mh.filetype == MH_EXECUTE) {
                NSLog(@"Found main binary mach-o image @ %p!", (void *)addr);
                PrintMachHeader64(mh);
            }
        }
        if (info.protection >= 0x5 && info.max_protection >= 0x5 && info.user_tag == 255 && info.share_mode == 1) {
            uint8_t buf[24] = {0};
            vm_size_t outSz = 0;
            kern_return_t r = vm_read_overwrite(task, (vm_address_t)addr, (vm_size_t)sizeof(buf), (vm_address_t)buf, &outSz);
            if (r == KERN_SUCCESS) {
                NSUInteger groupBytes = 4;           // 2字节=4 hex位
                NSString *sep = @" ";
                size_t len = sizeof(buf);
                NSMutableString *hex = [NSMutableString stringWithCapacity:len * (2 + sep.length / groupBytes)];
                for (size_t i = 0; i < len; i++) {
                    [hex appendFormat:@"%02X", buf[i]]; // 大写示例
                    if (((i + 1) % groupBytes) == 0 && i + 1 < len) {
                        [hex appendString:sep];
                    }
                }
                NSLog(@"Region %@ @0x%lx size=0x%lx prot=0x%x first bytes: %@", PrintVMRegionRecurseInfo64(info), addr, size, info.protection, hex);
            }
        }
        
        addr += size;
    }
}

static inline const char *mh_magic_str(uint32_t m) {
    switch (m) {
        case MH_MAGIC_64:  return "MH_MAGIC_64";
        case MH_CIGAM_64:  return "MH_CIGAM_64";
        case MH_MAGIC:     return "MH_MAGIC";
        case MH_CIGAM:     return "MH_CIGAM";
        default:           return "UNKNOWN";
    }
}

static inline const char *mh_filetype_str(uint32_t t) {
    switch (t) {
        case MH_OBJECT:      return "OBJECT";
        case MH_EXECUTE:     return "EXECUTE";
        case MH_DYLIB:       return "DYLIB";
        case MH_DYLINKER:    return "DYLINKER";
        case MH_BUNDLE:      return "BUNDLE";
        case MH_DSYM:        return "DSYM";
        case MH_KEXT_BUNDLE: return "KEXT";
        default:             return "UNKNOWN";
    }
}

static inline const char *mh_cputype_str(cpu_type_t c) {
#ifdef CPU_TYPE_ARM64
    if (c == CPU_TYPE_ARM64) return "ARM64";
#endif
#ifdef CPU_TYPE_X86_64
    if (c == CPU_TYPE_X86_64) return "X86_64";
#endif
    return "UNKNOWN";
}

static inline void PrintMachHeader64(struct mach_header_64 mh) {
    NSLog(@"mach_header_64 {");
    NSLog(@"  magic     = 0x%08x (%s)", mh.magic, mh_magic_str(mh.magic));
    NSLog(@"  cputype   = %d (%s)", mh.cputype, mh_cputype_str(mh.cputype));
    NSLog(@"  cpusubtype= %d", mh.cpusubtype);
    NSLog(@"  filetype  = %u (%s)", mh.filetype, mh_filetype_str(mh.filetype));
    NSLog(@"  ncmds     = %u", mh.ncmds);
    NSLog(@"  sizeofcmds= %u", mh.sizeofcmds);
    NSLog(@"  flags     = 0x%08x", mh.flags);
    NSLog(@"  reserved  = 0x%08x", mh.reserved);
    NSLog(@"}");
}
