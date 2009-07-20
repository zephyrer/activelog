================================================================================
    VAALOG 0.04.09 - Feb 04, 2007
================================================================================
1. Create MULTIUSE OLE (single threaded) Application
    - Use Regaut.exe program registers a running object as active in the           - DONE
      running object table (ROT)  http://support.microsoft.com/kb/q155690/
    - Change in VAALOG.idl for interface names                                     - DONE
    - Fix ProgID (http://support.microsoft.com/kb/q129095/)                        - DONE
    - WI0001 - CreateObject                                                        - DONE
       Regaut is MDI app and OnCloseDocument fail to clean up (remarks in code)   
       S1: New additional object. First attempt fail on COM call - becouse
       document not yet created. Is good to investigate ConnectTemplate.
       S2: If case that app is better to be hidden - needs to create storage
       class to keep data. It looks has to be done anyway
    - Safe for scripting ( by check, or search for xml security )                  - DONE
         http://support.microsoft.com/kb/161873
    - Single Instance                                                              - DONE
    - use try catch LogData call (in case if doc or frame is null)                 - DONE
      ( possible out of memory, or FrameWnd is NULL )
    - Select best ole support (Free-Threaded ???)                                  - DESIGN

2. UI interface
    - show item in left view ( no directories )                                    - DONE
    - show XML in right view                          (Review)                     - DONE
    - use standard scalable UpdateView processing                                  - DONE
    - remember last view settings ( Window size, SPLIT width )                     - DONE 
    - Rename Exit to Quit menu, add Hide or Minimize to Systray                    - DONE 
    - Start of second instance by user has to forward call to first                - DONE  
    - Get idea about Edit menu (Copy is usefull)                                   - DONE
		Remove Undo, Cut, Delete                                                   - DONE
    - Get idea about File menu (New,Save,Save As, Recent Files...)                 - DONE
        Remove menu Save                                                           - DONE
        Menu New will reset list                                                   - DONE
		Open menu Remove                                                           - DONE
    - Print menu                                                                   - DONE
		 
3. File System and Data structure	
    - trial version will use only in-memory List                                   - DONE
    - add recv time as parameter (part of RSS log)                                 - SKIP
    - dump data to file system (single/mult files or use RSS as log file)          - DESIGN
        + Open Log Files  ( RSS in mind )  
        + COM Messages
    - add support for showing HTML files (Use MSXML DOM doc)                       - DONE  
    - Add save item to temp folder background thread ( or use async DOM )          - SKIP
      - Use overlapped, async save ???
    - use background thread to process in-memory items data and save to            - SKIP
	  file system 
    - WI0002 - Use SAX for validating and saving xml                               - SKIP
    - Use DOM for validating and saving xml                                        - DONE

4. Registration ( static library ??? )
    - Copy flow                                                                    - DONE
    - generate 2 keys                                                              - DONE
    - select crypto method (AES)                                                   - DONE
    - WI0003 - Expiry CHECK                                                        - DONE
    - Prohibit reinstall of program after expiry (CustomAction DLL)                - SKIP
      uninstall will not delete reg start date info
      install will check this info for previous install 
      date and if so show registration message
      
    - Limit Evaluation version to 100 msg list entry                               - SKIP
    - Create generate key dll (access from c#)                                     - DONE

5. Setup app
    - use msi inside zip                                                           - DONE
    - Add registry settings under HKEY_CLASSES_ROOT                                - DONE
    - Add Implemeneted category reg settings                                       - SKIP

6. Future releases support 
	- WI0004 Change key structure                                                  - DONE	

	New key structure
	AL10B-247AY-QBPZV-9NQDF-I5V3I-8PCIA

7. Testing 
    - create test plan                                                             - WORKING
    - Test key by test ( loop with random names )                                  - WORKING

-----------------------------------------------------------------------------------

2.
    - Save application settings info to xml                                        - SKIP

        New  - could be treated as new List ( reset tree view, or add new folder )
        Save - could save list into single file (RSS) save should be automatic      
        Open - could open list from RSS
