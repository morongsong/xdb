

TheTargetName='xdb'
project (TheTargetName.."Test")
	  filter {"configurations:Debug"}
        targetname (TheTargetName..'Test'..'d')
    filter {"configurations:Release"}	
        targetname (TheTargetName..'Test')
    filter {} 
    kind 'ConsoleApp'
    

    files {'*.*'}

    includedirs {'../'..TheTargetName,"../"}
    

    
    
    pchheader ('stdafx.h')
    pchsource ('stdafx.cpp')
    
    
    
    
    