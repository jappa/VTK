if(NOT VTK_LEGACY_REMOVE)
  vtk_module(vtkFiltersMatlab
    EXCLUDE_FROM_WRAPPING
    EXCLUDE_FROM_WRAP_HIERARCHY
    EXCLUDE_FROM_ALL
    DEPENDS
      vtkCommonCore
      vtkCommonExecutionModel
      vtksys
    PRIVATE_DEPENDS
      vtkCommonDataModel
      vtkCommonMisc
    )
endif()
