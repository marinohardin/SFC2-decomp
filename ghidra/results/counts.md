## Functions

- total functions in the program: **44232** (function 27479, eh-funclet 16544, thunk 209)
- ordinary functions (excluding thunks and EH funclets): 27479, 3306300 bytes
- of those with a non-default name: **10624** (38.7%), 1368125 bytes (41.4% of function bytes)

| kind | name_source | functions |
|---|---|---|
| function | default | 16855 |
| eh-funclet | eh-analyzer | 16544 |
| function | rtti-class-recovery | 8223 |
| function | sdklib-exact | 1859 |
| function | ghidra-fid-vs | 307 |
| function | sdk-fid | 217 |
| thunk | default | 209 |
| function | ghidra-analysis | 17 |
| function | imported | 1 |

| ordinary functions by name quality | functions |
|---|---|
| class known, method placeholder (vfunctionN / ctor-or-dtor) | 4281 |
| class recovery: constructor / destructor / deleting destructor | 3942 |
| real symbol name (sdklib-exact) | 1859 |
| real symbol name (ghidra-fid-vs) | 307 |
| real symbol name (sdk-fid) | 217 |
| real symbol name (ghidra-analysis) | 17 |
| real symbol name (imported) | 1 |

## Per-step function counts

| state | functions | function bytes | default-named | class namespaces | bad-instruction bookmarks |
|---|---|---|---|---|---|
| after auto-analysis (step 1) | 34013 | 2091812 | 17160 | 2112 | 8 |
| after VC6 RTTI + gap fill (step 1b) | 44100 | 3465781 | 27230 | 2112 | 130 |
| after class recovery (step 1b) | 44173 | 3468627 | 17869 | 2108 | 130 |
| after SDK FID (step 2) | 44173 | 3468627 | 17118 | 2222 | 130 |
| after exact matches (step 3) | 44232 | 3475472 | 17064 | 2256 | 130 |

- gap fill: 6132 functions created over 8 passes; undefined .text bytes 1142300 -> 90409

## Library flag

| library | functions |
|---|---|
| (game code) | 26563 |
| stl | 562 |
| crt | 304 |
| wonapi | 210 |
| import-thunk | 41 |
| cryptopp | 8 |

## SDK Function ID (step 2)

| archive | programs | functions attempted | added to fidb | excluded | exclusions |
|---|---|---|---|---|---|
| ScriptInterfaceR.lib | 92 | 5105 | 3168 | 1937 | MEMORY_ACCESS_EXCEPTION=0 FAILED_FUNCTION_FILTER=0 IS_THUNK=36 DUPLICATE_INFO=1268 FAILS_MINIMUM_SHORTHASH_LENGTH=633 NO_DEFINED_SYMBOL=0 |
| SharedR.lib | 79 | 4390 | 2711 | 1679 | MEMORY_ACCESS_EXCEPTION=0 FAILED_FUNCTION_FILTER=0 IS_THUNK=31 DUPLICATE_INFO=1195 FAILS_MINIMUM_SHORTHASH_LENGTH=453 NO_DEFINED_SYMBOL=0 |
| cryptlibR.lib | 90 | 4672 | 2832 | 1840 | MEMORY_ACCESS_EXCEPTION=0 FAILED_FUNCTION_FILTER=0 IS_THUNK=28 DUPLICATE_INFO=790 FAILS_MINIMUM_SHORTHASH_LENGTH=1022 NO_DEFINED_SYMBOL=0 |
| dmp3D_MSVCR.lib | 8 | 81 | 73 | 8 | MEMORY_ACCESS_EXCEPTION=0 FAILED_FUNCTION_FILTER=0 IS_THUNK=2 DUPLICATE_INFO=0 FAILS_MINIMUM_SHORTHASH_LENGTH=6 NO_DEFINED_SYMBOL=0 |
| sfcTiger_MSVCR.lib | 48 | 3297 | 2363 | 934 | MEMORY_ACCESS_EXCEPTION=0 FAILED_FUNCTION_FILTER=0 IS_THUNK=13 DUPLICATE_INFO=235 FAILS_MINIMUM_SHORTHASH_LENGTH=686 NO_DEFINED_SYMBOL=0 |

- functions with an SDK FID hit (score >= 14.6): 1931; dispositions: single-renamed 894, single-named 751, multi 286
- functions whose final name comes from SDK FID: **217**

## Exact SDK matches (step 3)

- rows=3758, addresses=1934, names=2396, created=59, trimmed=0, create_failed=0, replaced_other_names=318, fid_agree=1427, fid_conflict=1, icf_addresses=170, labels_added=887, demangle_failed=3, section_only=74, skipped_overlap=1
- events: replaced 318, section-only 74, created 59, demangle-failed 3, overlaps-previous-match 1, fid-conflict 1
- functions whose final name comes from the exact table: **1859**

## Translation units (step 4)

- source-path strings referenced from code: 229 (227 .c/.cpp, 2 headers)
- functions with a TU from a path string they reference: **639** (7 reference more than one .cpp)
- functions with a TU inferred from both neighbours (same single TU before and after): 1019
- functions with only header paths: 37
- distinct TUs with at least one function: 226

## Classes (step 1b)

- classes (RTTI type descriptors / hierarchy descriptors / vftables): **2104**
- with a hierarchy descriptor (parents known): 1752 with at least one base; with vftables: 2033; with a recovered structure size: 1954
- vftables: 2149, slots: 27479, slots pointing at a named function: 27264

## Largest unnamed regions

| start | end | functions | bytes | named before | named after |
|---|---|---|---|---|---|
| 006bf590 | 006c842e | 154 | 29391 | 006b9bf0 CryptoPP::HashModule::CalculateDigest | 006c9980 tDynamicMemoryFreeList::vfunction21 |
| 006b9c40 | 006bf532 | 69 | 21465 | 006b9bf0 CryptoPP::HashModule::CalculateDigest | 006c9980 tDynamicMemoryFreeList::vfunction21 |
| 006ed020 | 006f2661 | 73 | 21361 | 006ecf80 WONAPI::BigInteger::BigInteger | 006f2670 WONAPI::WONAPIPumpThread::scalar_deleting_destructor |
| 0074096a | 0074531e | 81 | 18644 | 0074092e __fload | 0074531f Replicator::operator+= |
| 0072f110 | 00733533 | 95 | 16169 | 0072f020 CryptoPP::DSABase::MaxDigestLength | 00735c90 type_info::~type_info |
| 0052c8c8 | 005307d8 | 90 | 15846 | 0052c7d6 tNetMsg::~tNetMsg | 005307d9 tNetMsg_kAdjustSpeed::~tNetMsg_kAdjustSpeed |
| 00614210 | 00618923 | 102 | 15384 | 006141b0 twVolume::~twVolume | 00618930 tSoundSelection::mGetSoundPartHasNoMoreUseForThis |
| 006c99b0 | 006cdb42 | 103 | 15349 | 006c9980 tDynamicMemoryFreeList::vfunction21 | 006cdb50 WONAPI::AsyncSocket::AsyncSocket |
| 0056edc3 | 00572927 | 149 | 15205 | 0056edb9 nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction66 | 00572928 tTurn::tTurn |
| 006ad7d0 | 006b104b | 27 | 14312 | 006ad410 GetDXVersion | 006b1050 tSWView3D::vfunction98 |
| 005bd3b9 | 005c0bb2 | 82 | 14187 | 005bd370 nDatabase::tID::tID | 005c0bb3 tMapRelayS::tMapRelayS |
| 00530b70 | 0053430e | 49 | 14150 | 00530b28 tNetMsg_kScriptPrimitive::~tNetMsg_kScriptPrimitive | 0053430f tShipInGame::tShipInGame |
| 005f8558 | 005fb9aa | 111 | 13356 | 005f837f tMetaClientChatCtlrC::tMetaClientChatCtlrC_Constructor_or_Destructor | 005fb9ab tMemberCallback<tMetaClientChatCtlrC,tChatChannel_const&>::tMemberCallback<tMetaClientChatCtlrC,tChatChannel_const&> |
| 005c2394 | 005c5682 | 133 | 13006 | 005c1fef tMapSettings::~tMapSettings | 005c5683 tMapWatchdog::tMapWatchdog |
| 005b74b8 | 005ba478 | 71 | 12153 | 005b72f2 tMapCtlrS::~tMapCtlrS | 005bc519 nDatabase::tID::tID |
| 00582bcf | 005861fd | 133 | 12086 | 00582b13 tMissionMatcherCtlrS::~tMissionMatcherCtlrS | 005861fe tMissionMatcherRelayS::scalar_deleting_destructor |
| 0069aa00 | 0069e144 | 53 | 11961 | 0069a9f0 tApplication::vfunction18 | 0069e150 tD3DViewMgr::vfunction2 |
| 00746f7f | 0074a096 | 61 | 11540 | 00746f60 __allshl | 0074a097 ___add_12 |
| 006b5fe0 | 006b8fdc | 36 | 10563 | 006b5fa0 twDLLMgrBase::vfunction5 | 006b9000 twDisplayMgr::twDisplayMgr |
| 004b7161 | 004b99f6 | 21 | 10268 | 004b714d tState_Namespace_Tactical::tState_Tactical::vfunction7 | 004b99f7 tState_Namespace_Tactical::tState_Tactical::vfunction8 |
| 00445a14 | 0044823e | 49 | 10207 | 004459be tShipSystemMask::~tShipSystemMask | 0044823f tShipSystemsExt::tShipSystemsExt |
| 00733550 | 00735c77 | 44 | 9682 | 0072f020 CryptoPP::DSABase::MaxDigestLength | 00735c90 type_info::~type_info |
| 004fa526 | 004fc7fe | 33 | 8821 | 004fa0f1 tHypermass<36>::vfunction1 | 004fc7ff tFx_IonStorm::vfunction14 |
| 005a2b23 | 005a4d6e | 89 | 8618 | 005a2a02 tMemberCallback<tDataValidatorS,nSwitch::tAsyncProcedure**>::tMemberCallback<tDataValidatorS,nSwitch::tAsyncProcedure**>_Constructor_or_Destructor | 005a4d6f tDataWatchdog::tDataWatchdog |
| 0050bcc7 | 0050df19 | 58 | 8534 | 0050bc96 tBasicFileCache::vfunction11 | 0050df1a ParseTabData::ParseTabData |

## Timings and memory

| step | wall time | peak RSS | rc |
|---|---|---|---|
| 01_import | 154.38s | 3496MB | rc=0 |
| 01b_classes | 234.63s | 2098MB | rc=0 |
| 02a_import_libs | 25.85s | 991MB | rc=0 |
| 02a_analyze_libs | 199.07s | 2639MB | rc=0 |
| 02b_fidb | 9.59s | 1751MB | rc=0 |
| 02c_fid_apply | 11.55s | 2058MB | rc=0 |
| 03_exact | 5.94s | 649MB | rc=0 |
| 04_tu | 8.34s | 1671MB | rc=0 |
| 05_export | 16.04s | 1265MB | rc=0 |
