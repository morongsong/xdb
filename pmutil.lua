
--Ver 1.0
-- pm_copyfile
-- pm_lib_cpy_lib
-- pm_lib_use_lib


function pm_copyfile(srcfile,srcdir,destdir)
    --print(srcfile,srcdir,destdir)
    dest_with_file = path.translate(path.getabsolute(destdir..'/'..srcfile), "/")
    src_with_file = path.translate(path.getabsolute(srcdir..'/'..srcfile), "/")

    --print(dest_with_file)
    --print(src_with_file)
    if dest_with_file == src_with_file then
        return
    end

    destdirfx = path.translate(path.getabsolute(destdir), "/")
    os.mkdir(destdirfx)
    --print(destdirfx)

    bisfile = os.isfile(dest_with_file)

    if not bisfile then
        ok, err = os.copyfile(src_with_file,dest_with_file)
        if ok then
            --print('cp file '..srcfile..' from '..srcdir..' to '..destdirfx..'  ok...')
        else
            --print('cp file '..srcfile..' from '..srcdir..' to '..destdirfx..'  fail 1...')
        end
    else
        info = os.stat(dest_with_file)
        info2 = os.stat(src_with_file)
        --print(info,info2)
        if info and info2 then
            if (info.mtime==info2.mtime) and (info.size==info2.size) then
                --print('cp file '..srcfile..' from '..srcdir..' to '..destdirfx..'  same file ok...')
            else
                ok, err = os.copyfile(src_with_file,dest_with_file)
                if ok then
                    --print('cp file '..srcfile..' from '..srcdir..' to '..destdirfx..'  ok...')
                else
                    --print('cp file '..srcfile..' from '..srcdir..' to '..destdirfx..'  fail 2...')
                end
            end
        else
            --print('cp file '..srcfile..' from '..srcdir..' to '..destdir..'  fail 3...')
        end
    end
end


function pm_lib_cpy_lib(libname,libdir,dstdir)
    --dstdir = path.translate(path.getabsolute("."), "\\")
    pm_copyfile(libname..'.dll',libdir..'/Bin',dstdir..'/Bin')
    pm_copyfile(libname..'d.dll',libdir..'/Bin',dstdir..'/Bin')
    
    pm_copyfile(libname..'.pdb',libdir..'/Bin',dstdir..'/Bin')
    pm_copyfile(libname..'d.pdb',libdir..'/Bin',dstdir..'/Bin')   

    pm_copyfile(libname..'.dll',libdir..'/Binx64',dstdir..'/Binx64')
    pm_copyfile(libname..'d.dll',libdir..'/Binx64',dstdir..'/Binx64')

    pm_copyfile(libname..'.pdb',libdir..'/Binx64',dstdir..'/Binx64')
    pm_copyfile(libname..'d.pdb',libdir..'/Binx64',dstdir..'/Binx64')    
end

function pm_lib_link(TheLibDir,the_lib_name)
        filter {"platforms:Win32","configurations:Debug"}
            links{TheLibDir..'/Lib/'..the_lib_name..'d.lib'}
        filter {"platforms:Win32","configurations:Release"}
            links{TheLibDir..'/Lib/'..the_lib_name..'.lib'}
        filter {"platforms:x64","configurations:Debug"}
            links{TheLibDir..'/Libx64/'..the_lib_name..'d.lib'}
        filter {"platforms:x64","configurations:Release"}
            links{TheLibDir..'/Libx64/'..the_lib_name..'.lib'}
        filter {}
end

function pm_lib_use_lib(libname,drtype,libdir,targetdir,libdlldef)
    filter {}
    
    includedirs { libdir}
    
    the_out_libdll_dir = libdir
    the_lib_name=libname
    
    if drtype == "shared" then
        pm_lib_cpy_lib(libname,libdir,targetdir)

        filter {"platforms:Win32","configurations:Debug"}
            links{the_out_libdll_dir..'/Lib/'..the_lib_name..'d.lib'}
        filter {"platforms:Win32","configurations:Release"}
            links{the_out_libdll_dir..'/Lib/'..the_lib_name..'.lib'}
        filter {"platforms:x64","configurations:Debug"}
            links{the_out_libdll_dir..'/Libx64/'..the_lib_name..'d.lib'}
        filter {"platforms:x64","configurations:Release"}
            links{the_out_libdll_dir..'/Libx64/'..the_lib_name..'.lib'}
        filter {}
        defines {libdlldef}
    end  

    if drtype == "static" then   
        filter {"platforms:Win32","configurations:Debug"}
            links{the_out_libdll_dir..'/Lib/'..the_lib_name..'Static'..'d.lib'}
        filter {"platforms:Win32","configurations:Release"}
            links{the_out_libdll_dir..'/Lib/'..the_lib_name..'Static'..'.lib'}
        filter {"platforms:x64","configurations:Debug"}
            links{the_out_libdll_dir..'/Libx64/'..the_lib_name..'Static'..'d.lib'}
        filter {"platforms:x64","configurations:Release"}
            links{the_out_libdll_dir..'/Libx64/'..the_lib_name..'Static'..'.lib'}
        filter {}
    end
   
    filter {}
end


function pm_pj_lua()
project '_Pmake'
    kind 'Utility'

    files {'**.lua'}

    vpaths {["*"] = {'**.lua'}}

    postbuildcommands {
      "cd /d ..\\",
      "Premake5 ".._ACTION
    }
    cleancommands {"cd /d ..\\"}
end