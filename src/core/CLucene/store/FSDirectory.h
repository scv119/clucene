/*------------------------------------------------------------------------------
* Copyright (C) 2003-2006 Ben van Klinken and the CLucene Team
*
* Distributable under the terms of either the Apache License (Version 2.0) or
* the GNU Lesser General Public License, as specified in the COPYING file.
------------------------------------------------------------------------------*/
#ifndef _lucene_store_FSDirectory_
#define _lucene_store_FSDirectory_


#include "Directory.h"
#include "IndexInput.h"
#include "IndexOutput.h"
#include <string>
#include <vector>

//#include "Lock.h"
//#include "LockFactory.h"
//#include "CLucene/util/VoidMap.h"
CL_CLASS_DEF(util,StringBuffer)

   CL_NS_DEF(store)

   /**
   * Straightforward implementation of {@link lucene::store::Directory} as a directory of files.
   * <p>If the system property 'disableLuceneLocks' has the String value of
   * "true", lock creation will be disabled.
   *
   * @see Directory
   */
	class CLUCENE_EXPORT FSDirectory:public Directory{
	private:
		class FSIndexOutput;
		class FSIndexInput;
		friend class FSDirectory::FSIndexOutput;
		friend class FSDirectory::FSIndexInput;

	protected:
		FSDirectory(const char* path, const bool createDir, LockFactory* lockFactory=NULL);
	private:
    std::string directory;
		int refCount;
		void create();

		static const char* LOCK_DIR;
		static const char* getLockDir();
		char* getLockPrefix() const;
		static bool disableLocks;

		void priv_getFN(char* buffer, const char* name) const;
		bool useMMap;

	protected:
		/// Removes an existing file in the directory.
		bool doDeleteFile(const char* name);

	public:
	  ///Destructor - only call this if you are sure the directory
	  ///is not being used anymore. Otherwise use the ref-counting
	  ///facilities of _CLDECDELETE
		~FSDirectory();

		/// Get a list of strings, one for each file in the directory.
		bool list(std::vector<std::string>* names) const;

		/// Returns true iff a file with the given name exists.
		bool fileExists(const char* name) const;

      /// Returns the text name of the directory
		const char* getDirName() const; ///<returns reference


    /**
    Returns the directory instance for the named location.

    Do not delete this instance, only use close, otherwise other instances
    will lose this instance.

    <p>Directories are cached, so that, for a given canonical path, the same
    FSDirectory instance will always be returned.  This permits
    synchronization on directories.

    @param file the path to the directory.
    @param create if true, create, or erase any existing contents.
    @return the FSDirectory for the named file.
    */
		static FSDirectory* getDirectory(const char* file, const bool create=false, LockFactory* lockFactory=NULL);

		/// Returns the time the named file was last modified.
		int64_t fileModified(const char* name) const;

		//static
		/// Returns the time the named file was last modified.
		static int64_t fileModified(const char* dir, const char* name);

		//static
		/// Returns the length in bytes of a file in the directory.
		int64_t fileLength(const char* name) const;

		/// Returns a stream reading an existing file.
		bool openInput(const char* name, IndexInput*& ret, CLuceneError& err, int32_t bufferSize=-1);

		IndexInput* openMMapFile(const char* name, int32_t bufferSize=LUCENE_STREAM_BUFFER_SIZE);

		/// Renames an existing file in the directory.
		void renameFile(const char* from, const char* to);

      	/** Set the modified time of an existing file to now. */
      	void touchFile(const char* name);

		/// Creates a new, empty file in the directory with the given name.
		///	Returns a stream writing this file.
		IndexOutput* createOutput(const char* name);

		  ///Decrease the ref-count to the directory by one. If
		  ///the object is no longer needed, then the object is
		  ///removed from the directory pool.
      void close();

	  /**
	  * If MMap is available, this can disable use of
	  * mmap reading.
	  */
	  void setUseMMap(bool value);
	  /**
	  * Gets whether the directory is using MMap for inputstreams.
	  */
	  bool getUseMMap() const;

	  std::string toString() const;

		static const char* getClassName();
		const char* getObjectName() const;

	  /**
	  * Set whether Lucene's use of lock files is disabled. By default,
	  * lock files are enabled. They should only be disabled if the index
	  * is on a read-only medium like a CD-ROM.
	  */
	  static void setDisableLocks(bool doDisableLocks);

	  /**
	  * Returns whether Lucene's use of lock files is disabled.
	  * @return true if locks are disabled, false if locks are enabled.
	  */
	  static bool getDisableLocks();

  };

CL_NS_END
#endif
