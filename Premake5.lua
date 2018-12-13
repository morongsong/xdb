

dofile('pmutil.lua')
TheTargetName='xdb'
TheTargetName_Dll_ExpDef='XDB_EXPORT'
TheTargetName_Dll_ImpDef='XDB_IMPORT'

local TheCurSlnName=TheTargetName

workspace (TheCurSlnName)
    configurations {'Debug', 'Release'}
    platforms {"Win32","x64"}
    location ('_Build')
    filter "platforms:Win32"
      architecture 'x32'
    filter "platforms:x64"
      architecture 'x64'
          
    filter {"platforms:Win32"}
      targetdir ("Bin")
      implibdir ("Lib")
      debugdir  ("Bin")
    filter {"platforms:x64"}
      targetdir ("Binx64")
      implibdir ("Libx64")
      debugdir  ("Binx64")
    filter {'kind:StaticLib',"platforms:Win32"}
        targetdir ("Lib")
    filter {'kind:StaticLib',"platforms:x64"}
        targetdir ("Libx64")  

    filter {"configurations:Debug"}
      defines { "DEBUG","_DEBUG"}
      symbols "On"
    filter {"configurations:Release"}	
      defines { "NDEBUG" }
      optimize "On"
      symbols "On"
      

pm_pj_lua()

os.chdir("src")

--[[
project (TheTargetName)
    language 'C++'
	filter {"configurations:Debug"}
        targetname (TheTargetName..'d')
    filter {"configurations:Release"}	
        targetname (TheTargetName)
    filter {} 
    kind 'SharedLib'
    includedirs {'../'..TheTargetName,"../"}
    files {'../'..TheTargetName..'/**','**'}
    defines {TheTargetName_Dll_ExpDef}
 ]]--   

project (TheTargetName..'Static')
    language 'C++'
	filter {"configurations:Debug"}
        targetname (TheTargetName..'Static'..'d')
    filter {"configurations:Release"}	
        targetname (TheTargetName..'Static')
    filter {} 
    kind 'StaticLib'
    includedirs {'../'..TheTargetName,"../"}
	files {'**'}
    files {'../'..TheTargetName..'/**'}
	--files {'../'..'snappy'..'/**'}
    
    pchheader ('stdafx.h')
    pchsource ('stdafx.cpp')

os.chdir("../")
os.chdir("test")

project (TheTargetName.."Test")
	filter {"configurations:Debug"}
        targetname (TheTargetName..'Test'..'d')
    filter {"configurations:Release"}	
        targetname (TheTargetName..'Test')
    filter {} 
    kind 'ConsoleApp'
    
    dependson(TheTargetName,TheTargetName..'Static')
    files {'*.*'}

    includedirs {'../'..TheTargetName,"../"}
    
    
    pm_lib_use_lib('xdb','static','../','./',TheTargetName_Dll_ImpDef)
    
    
    pchheader ('stdafx.h')
    pchsource ('stdafx.cpp')
    
    
    
    
    