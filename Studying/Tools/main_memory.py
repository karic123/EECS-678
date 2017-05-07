import locale, numpy
locale.setlocale(locale.LC_ALL, 'en_US.UTF-8')

# ----------------------------------------------------

def FORMAT( val ):
    return locale.format("%d", val, grouping=True )

def get_bytes_to_MB( byteVal ):
    return ( byteVal ) / 1024 / 1024


def get_bytes_to_KB( byteVal ):
    return ( byteVal ) / 1024


def get_GB_to_bytes( gbVal ):
    return gbVal * 1024 * 1024 * 1024


def get_GB_to_KB( gbVal ):
    return gbVal * 1024 * 1024

# ----------------------------------------------------
# Address Translation

#   8 bits      8 bits      8 bits
# ----------------------------------
# | 1st level | 2nd level | offset |
# ----------------------------------

# Virtual address format: 24 bits

#   4 bits    3 bits  1 bit
# ------------------------
# | frame # | unused | V |
# ------------------------ 

# Page table entry: 8 bits

def get_size_of_single_page( offset_bits ):
    return 2 ** offset_bits # 2^offset_bits


def get_virtual_address_format_bits( level_1_bits, level_2_bits, offset_bits ):
    return level_1_bits + level_2_bits + offset_bits


def get_size_of_virtual_address_space_bytes( virtual_address_format_bits ):
    return 2 ** virtual_address_format_bits # 2^virtual_address_format_bits


def get_size_of_physical_address_space_bytes( frame_bits, page_table_entry_bits ):
    return ( 2 ** frame_bits ) * ( 2 ** page_table_entry_bits ) # 2^frame_bits + 2^page_table_entry_bits


# ----------------------------------------------------

first_level_bits        = 8
second_level_bits       = 8
offset_bits             = 8

page_table_entry_bits   = 8
frame_bits              = 4

virtual_address_format_bits = get_virtual_address_format_bits( first_level_bits, second_level_bits, offset_bits )
virtual_address_space_bytes = get_size_of_virtual_address_space_bytes( virtual_address_format_bits )

single_page_bytes = get_size_of_single_page( offset_bits )

size_of_physical_address_space_bytes = get_size_of_physical_address_space_bytes( frame_bits, page_table_entry_bits )


print( "1st level page size (bits):     " + str( first_level_bits ) )
print( "2nd level page size (bits):     " + str( second_level_bits ) )
print( "Size of offset (bits):          " + str( offset_bits ) )

print( "" )
print( "Page table entry size (bits):   " + str( page_table_entry_bits ) )
print( "Frame size (bits):              " + str( frame_bits ) )

print( "" )
print( "Size of a single page (bytes):  " + str( single_page_bytes ) )
print( "Virtual address format (bits):  " + str( virtual_address_format_bits ) )
print( "Virtual address space (bytes):  " + FORMAT( virtual_address_space_bytes ) + " (" + str( get_bytes_to_MB( virtual_address_space_bytes ) ) + " MB)" )
print( "Physical address space (bytes): " + FORMAT( size_of_physical_address_space_bytes ) + " (" + str( get_bytes_to_KB( size_of_physical_address_space_bytes ) ) + " KB)" )


# ----------------------------------------------------
# Page count and table size

# How many pages are needed for 4 GB (of physical memory...?) @ 32 bit?
# 4 GB / 4 KB = 1 Million Pages << I don't know how the 4 KB came up...

def get_pages_needed( memory_in_GB, magic_number_KB ):
    memory_KB = get_GB_to_KB( memory_in_GB )
    return memory_KB / magic_number_KB

# What is the required page table size? x GB (of physical memory...?) @ y bit

def get_page_table_size( pages_needed, something_bits ):
    something_bytes = something_bits / 8
    return pages_needed * something_bytes


physical_memory_GB  = 4
magic_number_KB     = 4
something_bits      = 32

pages_needed = get_pages_needed( physical_memory_GB, magic_number_KB )
page_table_size = get_page_table_size( pages_needed, something_bits )

print( "" )
print( "Physical memory (GB): " + str( physical_memory_GB ) + " @ " + str( something_bits ) + " bits" )
print( "Pages needed:         " + FORMAT( pages_needed ) )
print( "Page table size:      " + FORMAT( page_table_size ) + " (" + str( get_bytes_to_MB( page_table_size ) ) + " MB)" )



# ----------------------------------------------------
# Calculate offset

def get_offset_size_bits( page_size_KB ):
    # This needs to be in terms of bits...
    offset_bits = numpy.log2( page_size_KB )
    return offset_bits

page_size_KB = 4
offset_bits = get_offset_size_bits( page_size_KB )

print( "" )
print( "Page size (KB): " + str( page_size_KB ) )
print( "Offset bits:    " + str( offset_bits ) )



# ----------------------------------------------------
# Entries in a page table

def get_entries_in_page_table( index_bits ):
    return 2 ** index_bits # 2^index_bits


index_bits = 8
entries = get_entries_in_page_table( index_bits )

print( "" )
print( "Index bits:             " + str( index_bits ) )
print( "Entries in page table:  " + str( entries ) )




# ----------------------------------------------------
# Page number

# Given some logical address *(a<sub>)16</sub>* (hexadecimal #)
# and a page size of ```p``` bytes, what is the page number?

def get_page_number( page_size_bytes, address ):
    page_size_bits = numpy.log2( page_size_bytes )
    # Each place in hex is 4 bits, so the first two #s will be the page #
    page_num = address[:2]
    return page_num
    

page_size_bytes = 2048
address = "0703fe"
page_number = get_page_number( page_size_bytes, address )

print( "" )
print( "Page size (bytes): " + str( page_size_bytes ) )
print( "Address:           " + address )
print( "Page number (HEX): " + page_number )






