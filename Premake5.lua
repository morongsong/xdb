

dofile('pmutil.lua')

local TheCurSlnName='xdb'

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
dofile('test/Premake5.lua')
    
    
    