/**
 * @page DocTag_FAQ FAQ
 *
 * -  <b>Q</b>: why it takes so long to build\n
 *   <b>A</b>: may be:
 *   -  we need to git clone some repos to build the project,
 *     which may cost much time according to your network
 *   -  the C++ itself takes long time to compile
 *
 *   once you successfully build the library,
 *   it should be faster for the second time
 *
 * -  <b>Q</b>: compile error\n
 *   <b>A</b>: try running zfcleanup.sh/zfcleanup.bat and build again\n
 *   if error still occurred:
 *   -# try running "~/zfsetup.sh" to check whether it setup properly
 *   -# try running proper release script under "~/tools/release",
 *     and check the error message
 *
 * -  <b>Q</b>: no semantic completion in lua\n
 *   <b>A</b>: unfortunately, there is no suitable IDE to complete cpp contents in lua files,
 *   it's recommended to setup keyword completion by using #ZFDynamic::exportTag,
 *   and you may consider manually append some extra contents to match
 *   Exuberant Ctags format (http://ctags.sourceforge.net/FORMAT)
 *   @code
 *     ClassData	dummy	/^.$/;"		language:lua
 *   @endcode
 *   with the tag file, you may use it in vim or other editor envs
 *   to supply keyword completion
 *
 */

