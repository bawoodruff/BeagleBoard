@perl -x "%~f0" %*
@goto :eof
#!perl
#line 5
use strict;
use warnings;

use v5.14;
use Getopt::Long;
use Pod::Usage;
use File::Compare;

my ($buildPath, $clean, $ddkPath, $msvcPath, $sections, $excludeSections, $sdkPath, $sourceFile);

my $help = '';
GetOptions(
	'build-path|build:s'	=>	\$buildPath,
	'clean'					=>  \$clean,
	'ddk-path|ddk:s'		=>  \$ddkPath,
	'exclude-sections:s{,}'	=>	\$excludeSections,	
	'help|?'				=> 	\$help,
	'msvc-path|msvc:s'		=>	\$msvcPath,
	'sections:s{,}'			=>	\$sections,
	'sdk-path|sdk:s'		=>	\$sdkPath,
    '<>'					=>  sub { $sourceFile = shift;}
    ) or pod2usage(2);
    
pod2usage(-exitstatus => 0, -verbose => 2) if $help;

#
# Creates a list of files to be processed
# Returns the list or empty list in case of failure 
# Does not validate the files (whether they exist or not)
#
sub GetFilesToProcess {
    my $sourceFile = $_[0];
    my @filesToProcess;

    my @tmp = ReadListFile($sourceFile);
    if (@tmp) 
    {
        push @filesToProcess, @tmp;
    }
    else 
    {
       print("ERROR: Unable to read the list file [$sourceFile]");
       return;
    }

    return @filesToProcess;
}

#
# Reads the contents of a given text file
# Returns each line in the file or an empty list in case of failure 
#
sub ReadListFile {
    my $listFile = shift;
    $listFile =~ s/^\@//;

    if( !open(LST, "<$listFile" )) {
        return;
    }

    chomp (my @contents = <LST>);
    close LST;

    my @filesList;

    # Clean up the lines
    foreach (@contents) {
        s/#.*$//;
        s/^\s*//;
        s/\s*$//;
        if ($_)
        {
            push (@filesList, $_);
        }            
    }

    return @filesList;
}

sub RunCommand
{
    my ($command) = shift;

    # pipe the output
    open CMD, $command . " |";

    while (<CMD>)
    {
        chomp;
        print "$_\n";
    }
    close CMD;

    return $? >> 8;
}

sub CopyFile
{
    my $srcFileSpec = $_[ 0 ];
    my $dstFileName = $_[ 1 ];
    my $srcPath = $_[ 2 ];
    my $dstPath = $_[ 3 ];
    my $targetDir = $_[ 4 ];
    my $fileLog = $_[ 5 ];
    my $fileWarn = $_[ 6 ];

    my $dstDir = $dstPath . $targetDir;
    my $dstFile = $dstDir . $dstFileName;

    my $srcFile = $srcPath . "\\" . $srcFileSpec; 
    my $error = 0;

    print $fileLog "$srcFile $dstFile\n";
#    print "$srcFile $dstFile\n";

    if (-f( $srcFile ) || ( $srcFileSpec =~ m/\*/))
    {
        if (! -d $dstDir)
        {
            print "md $dstDir\n";
            `md $dstDir`;
        }
        
        print "copy \"$srcFile\" \"$dstFile\"\n";

        if ($srcFileSpec =~ m/\*/)
        {
            $error = RunCommand( "xcopy /y \"$srcFile\" \"$dstFile\" 2>&1" );
        }
        else
        {
            $error = RunCommand( "copy /y \"$srcFile\" \"$dstFile\" 2>&1" );
        }
            

        if ($error)
        {
            print "*** WARNING: failed to copy $srcFile ($error).\n";
            print $fileWarn "*** WARNING: failed to copy $srcFile ($error).\n";
        }
    }
    else
    {
        print "*** WARNING: $srcFile does not exist.\n";
        print $fileWarn "*** WARNING: $srcFile does not exist.\n";
    }
}

#------------------------------------------------------------------
#
# The main script starts here
#
#------------------------------------------------------------------

my ($execDrive, $execPath, $execName);

if (!$sourceFile)
{
    #
    # Nothing specified for source, default to MSVCToolChainForARM.ini
    #

    $execName = "MSVCToolChainForARM.ini";

    ($execDrive, $execPath) = File::Spec->splitpath($0);


    $sourceFile = $execDrive . $execPath . $execName ;

    print "*** No source files specified,\n    using default list: $sourceFile\n";
}
else
{
    ($execDrive, $execPath, $execName) = File::Spec->splitpath($sourceFile);
}

my @filesToProcess = GetFilesToProcess($sourceFile);

if (!@filesToProcess) {
    die "Failed to create the list of files to be processed. Exiting...";
}


my ($srcPath, $dstPath);

my %srcPathMap = (
    ddk     => 'C:\\WinDDK\\7600.16385.1',
    msvc    => 'C:\\Program Files (x86)\\Microsoft Visual Studio 11.0',
    sdk     => 'C:\\Program Files (x86)\\Microsoft SDKs\\Windows\\v7.0A'
    );

my $fileNameOnly = $execName;
$fileNameOnly =~ s/(.*)\.(.*)/$1/;

$dstPath = $ENV{BEAGLE_BUILD_PATH};
if (!$dstPath)
{
	die "ERROR: BEAGLE_BUILD_PATH is not defined.";
}

open(my $fileLog,  ">", $execDrive . $execPath . $fileNameOnly . ".log");
open(my $fileWarn, ">", $execDrive . $execPath . $fileNameOnly . ".wrn");

# clean destination

if ($clean)
{
	foreach my $toolDirectories('amd64', 'sdk', 'vc', 'x86')
	{	
        my $dstSectionPath = $dstPath . '\\' . $toolDirectories;
        		
    	print "Clean: deleting files from $dstSectionPath\n";
    	`rd /S /Q $dstSectionPath 2>&1`;		
	}	
}
    
foreach my $file (@filesToProcess) 
{
    if ($file)
    {
        (my $targetSection, $file, my $targetDir) = split(/,/, $file);
      
        # remove trailing spaces from targetSection
        $targetSection =~ s/\s+$//;
        
        if ($sections)
        {
            # if sections are specified and if this targetSection does not match, skip
            next if ($sections !~ /\b$targetSection\b/i);
        }

        if ($excludeSections)
        {
            # if the targetSection is in the excluded sections, skip it
            next if ($excludeSections =~ /\b$targetSection\b/i); 
        }

        if (!$file)
        {
            print "ERROR: file not specified\n";
            exit( -1 );
        }

        # remove trailing and leading spaces from file
        $file =~ s/\s+$//;  
        $file =~ s/^\s+//;
        
        # remove leading spaces from targetdir
        $targetDir =~ s/^\s+//;
   
        if (($targetDir eq ".") || ($targetDir eq ""))
        {
            $targetDir = "\\";
        }
        else
        {
            $targetDir = "\\" . $targetDir . "\\";
        }

		$srcPath = $srcPathMap{ $targetSection };
		if (!$srcPath)
		{
            print "ERROR: unknown section declared: $targetSection\n";
            exit( -1 );			
		} 
		         
        my ($dstFileName) = ".";
                
        CopyFile( $file, $dstFileName, $srcPath, $dstPath, $targetDir, $fileLog, $fileWarn );
    }
}
   
close($fileLog);
close($fileWarn);

#
# Check .wrn file length, pop open Notepad.exe if is non-zero
#

my $warningLogPath = $execDrive . $execPath . $fileNameOnly . ".wrn";

if (-s $warningLogPath)
{
    system( "start notepad.exe $warningLogPath" );
}


1; 
__END__ 
=head1 
NAME CopyBuildToolset.pl - Populates the build environment for the MSVC ARM build tool chain.
=head1 SYNOPSIS ./CopyBuildToolset.pl 
=head1 DESCRIPTION Populates the build environment for the MSVC ARM build tool chain.
--flashbuild specifies from which build to copy the XDK used by flash- live client branch (or requires XENON_FLASH_BUILD setting).
--help This help text
=head1 
AUTHOR Bryan A. Woodruff
