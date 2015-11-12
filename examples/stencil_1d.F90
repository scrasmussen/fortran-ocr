!
! Author: Soren Rasmussen
! University of Oregon
!

module m
  use ISO_C_BINDING
  use :: ocr_types
  use :: ocr_interfaces
  implicit none
  integer(C_INT64_T), parameter :: NPOINTS = 4
  integer(C_INT64_T), parameter :: NRANKS = 4
  integer(C_INT64_T), parameter :: NTIMESTEPS = 10
  integer(C_INT64_T), parameter :: NTIMESTEPS_SYNC = 5
  integer(C_INT64_T), parameter :: ITIMESTEP0 = 1
  integer(C_INT64_T), parameter :: HALO_RADIUS = 1
  real, parameter :: NORM = 1E-6
  ! Maybe add ifndefs for MIN, MAX, and ABS
  
  ! TO REMEMBER ocrGuid_t type == integer(C_INTPTR_T)

  type MyOcrTaskStruct_t
     type(ocrEdt_t) :: FNC
     integer(C_INTPTR_T) :: TML, OET, EDT
  end type MyOcrTaskStruct_t

  type gSettingsH_t
     type(C_PTR) :: ptr
     integer(C_INT64_T) :: NP, NR ! global problem size, number of ranks
     integer(C_INT64_T) :: NT, NT_SYNC, IT0
     integer(C_INT64_T) :: HR
  end type gSettingsH_t

  type settingsH_t
     integer(C_INT64_T) :: id, lb, ub, np ! rank, local bounds, local points
     integer(C_INT64_T) :: itimestep
  end type settingsH_t
  
  type dataH_t
     integer(C_INTPTR_T) :: DBK_xIn, DBK_xOut
  end type dataH_t

  type eventH_t
     integer(C_INTPTR_T) :: EVT_Lsend_fin, EVT_Rsend_fin
     integer(C_INTPTR_T) :: EVT_Lrecv_start, EVT_Rrecv_start
     integer(C_INTPTR_T) :: EVT_Lrecv_fin, EVT_Rrecv_fin
  end type eventH_t

  type rankH_t
     integer(C_INTPTR_T) :: DBK_settingsH
     integer(C_INTPTR_T) :: DBK_dataH
     integer(C_INTPTR_T), dimension(2) :: DBK_eventHs
  end type rankH_t

  type globalH_t
     integer(C_INT64_T) :: itimestep ! tag
     integer(C_INTPTR_T) :: DBK_gSettingsH ! commandline paramters/settings
     integer(C_INTPTR_T) :: DBK_gSettingsHs !-> broadcasted params/settings 
                                            !   gSettingsH_t[]
     integer(C_INTPTR_T) :: DBK_rankHs ! --> rankH_t[]
  end type globalH_t

contains

function FNC_settingsInit(paramc, paramv, depc, depv) result(returnGuid) &
     BIND(c)
  use ISO_C_BINDING
  implicit none
  integer(C_INTPTR_T) :: returnGuid
  integer(C_INT32_T), intent(IN) :: paramc, depc
  integer(C_INT64_T), intent(IN), dimension(*) :: paramv
  type(ocrEdtDep_t), intent(IN), dimension(*), target :: depv
  type(gSettingsH_t) :: PTR_gSettingsH_0

  PTR_gSettingsH_0%ptr = depv(1)%ptr
  PTR_gSettingsH_0%NP = paramv(1)
  PTR_gSettingsH_0%NR = paramv(2)
  PTR_gSettingsH_0%NT = paramv(3)
  PTR_gSettingsH_0%NT_SYNC = paramv(4)
  PTR_gSettingsH_0%IT0 = paramv(5)
  PTR_gSettingsH_0%HR = paramv(6)
  call printf_str("===paramv test" // C_NULL_CHAR);
  call printf_i("paramv(1) = " // C_NULL_CHAR, paramv(1));
  call printf_i("paramv(2) = " // C_NULL_CHAR, paramv(2));
  call printf_i("paramv(3) = " // C_NULL_CHAR, paramv(3));
  call printf_i("paramv(4) = " // C_NULL_CHAR, paramv(4));
  call printf_i("paramv(5) = " // C_NULL_CHAR, paramv(5));
  call printf_i("paramv(6) = " // C_NULL_CHAR, paramv(6));

  call printf_i("=In FNC_settingsInit" // C_NULL_CHAR, PTR_gSettingsH_0%HR)
  returnGuid = NULL_GUID
end function FNC_settingsInit

function FNC_globalInit(paramc, paramv, depc, depv) result (returnGuid) &
     BIND(c)
  use ISO_C_BINDING
  implicit none
  integer(C_INTPTR_T) :: returnGuid
  integer(C_INT32_T), intent(IN) :: paramc, depc
  integer(C_INT64_T), intent(IN), dimension(*) :: paramv
  type(ocrEdtDep_t), intent(IN), dimension(*), target :: depv
  integer(C_INT32_T) :: s_paramc, s_depc, s_idep, NR, ITO
  integer(C_INTPTR_T) :: DBK_gSettningsH_0, DBK_globalH
  type(C_PTR) :: PTR_gSettingsH_0, PTR_globalH, PTR_gSettingsH
  call printf_str("=In FNC_globalInit" // C_NULL_CHAR)
  

  returnGuid = NULL_GUID
end function FNC_globalInit

function mainEdt(paramc, paramv, depc, depv) result(returnGuid) &
     BIND (c, name='mainEdt')
  integer(C_INTPTR_T) :: returnGuid
  integer(C_INT32_T), intent(in) :: paramc, depc
  integer(C_INT64_T), intent(in), dimension(*) :: paramv
  type(ocrEdtDep_t), intent(in), dimension(*) :: depv ! add target? I think no
  ! variables
  ! INVESTIGATE why did I need integer(C_INT32_T) :: _paramc, _depc, _idep
  type(C_PTR) :: programArgv
  integer(C_INT32_T) :: argc, i, us_paramc, us_depc, us_idep
  ! ocrGuid_t
  integer(C_INTPTR_T) :: DBK_gSettingsH_0, TS_settingsInit_OET, affinity
  integer(C_INTPTR_T) :: DBK_globalH, TS_globalInit_OET, TS_globalCompute_OET
  integer(C_INTPTR_T) :: TS_globalFinalize_OET
  type(C_PTR) :: PTR_gSettingsH_0
  type(gSettingsH_t) :: gSettingsH_t_arg
  integer(C_INT16_T) :: false = 0
  type(MyOcrTaskStruct_t) :: TS_settingsInit, TS_globalInit, TS_globalCompute
  type(MyOcrTaskStruct_t) :: TS_globalFinalize
  integer(C_INT64_T), dimension(*) :: settingsInit_paramv(6)
  integer(C_INTPTR_T), dimension(1) :: testNULL
  integer(C_INT64_T), dimension(1) :: testNULLINT
  integer(C_INT8_T) :: status
  type(C_PTR) :: PTR_globalH
  type(globalH_t) :: sizeofGlobalH_t
  integer(C_INTPTR_T), dimension(1), parameter :: C_NULL_ARRAY = 0
  integer(C_INT64_T), dimension(1), parameter :: C64_NULL_ARRAY = 0


  programArgv = depv(1)%ptr
  argc = getArgc(programArgv)

  if (argc == 7) then
     i = 1
     call printf_str("ARGC == 7: NEED TO PROGRAM CONVERSIONS" // C_NULL_CHAR)
     ! == ATOI SECTION THAT NEEDS FURTHER INVESTIGATION ==
  end if
  call printf_i("npoints " // C_NULL_CHAR, npoints)
  call printf_i("nranks " // C_NULL_CHAR, nranks)
  call printf_i("ntimesteps " // C_NULL_CHAR, ntimesteps)
  call printf_i("ntimesteps_sync " // C_NULL_CHAR, ntimesteps_sync)
  call printf_i("itimestep0 " // C_NULL_CHAR, itimestep0)
  call printf_i("halo_radius " // C_NULL_CHAR, halo_radius)

  
  !===SETTINGS INIT PHASE===
  affinity = NULL_GUID
  call ocrDbCreate(DBK_gSettingsH_0, PTR_gSettingsH_0, &
       sizeof(gSettingsH_t_arg), DB_PROP_NONE, affinity, NO_ALLOC)
  call ocrEventCreate(TS_settingsInit_OET, OCR_EVENT_STICKY_T, false)
  settingsInit_paramv = (/NPOINTS, NRANKS, NTIMESTEPS, NTIMESTEPS_SYNC, &
       ITIMESTEP0, HALO_RADIUS/)
  TS_settingsInit%FNC%ptr = C_FUNLOC(FNC_settingsInit)
  call ocrEdtTemplateCreate(TS_settingsInit%TML, TS_settingsInit%FNC, 6, 1, &
       C_NULL_CHAR)

  !=====THIS NEEDS TO BE FIXED, TESTNULL IS NOT SENDING NULLPTR LIKE IT SHOULD
  call ocrEdtCreate( TS_settingsInit%EDT, TS_settingsInit%TML, EDT_PARAM_DEF, &
       settingsInit_paramv, EDT_PARAM_DEF, C_NULL_ARRAY, EDT_PROP_FINISH, &
       NULL_GUID, TS_settingsInit%OET)


  status = ocrAddDependence(TS_settingsInit%OET, TS_settingsInit_OET, &
       0, DB_MODE_NULL)

  us_idep = 0  
  status = ocrAddDependence(DBK_gSettingsH_0, TS_settingsInit%EDT, us_idep, &
       DB_MODE_RW)
  us_idep = us_idep + 1

  !===GLOBAL INIT PHASE===
  affinity = NULL_GUID
  call ocrDbCreate(DBK_globalH, PTR_globalH, sizeof(sizeofGlobalH_t), &
       DB_PROP_NONE, affinity, NO_ALLOC)
  call ocrEventCreate(TS_globalInit_OET, OCR_EVENT_STICKY_T, false)
  call ocrEventCreate(TS_globalCompute_OET, OCR_EVENT_STICKY_T, false)
  call ocrEventCreate(TS_globalFinalize_OET, OCR_EVENT_STICKY_T, false)
  
  TS_globalInit%FNC%ptr = C_FUNLOC(FNC_globalInit)
  call ocrEdtTemplateCreate(TS_globalInit%TML, TS_globalInit%FNC, 0, 3, &
       C_NULL_CHAR)

  call ocrEdtCreate(TS_globalInit%EDT, TS_globalInit%TML, EDT_PARAM_DEF, &
       C64_NULL_ARRAY, EDT_PARAM_DEF, C_NULL_ARRAY, EDT_PROP_FINISH, &
       NULL_GUID, TS_globalInit%OET)

  



  call printf_str("Fin" // C_NULL_CHAR)
  call ocrShutdown()
  returnGuid = 0 ! C_NULL_PTR
end function mainEdt

end module m
