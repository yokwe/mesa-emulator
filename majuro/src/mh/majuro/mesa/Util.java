package mh.majuro.mesa;

import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.util.Enumeration;
import java.util.LinkedList;
import java.util.List;

public final class Util {
	// Byte
	public static int asInt(byte value) {
		return value & 0xff;
	}
	// Cardinal
	public static int asInt(short value) {
		return value & 0xffff;
	}
	// Long Cardinal
	public static long asLong(int value) {
		return value & 0xffffffffL;
	}
	
	// Long
	public static int LowHalfAsInt(int value) {
		return value & 0xffff;
	}
	public static short LowHalf(int value) {
		return (short)LowHalfAsInt(value);
	}
	public static int HighHalfAsInt(int value) {
		return (value >>> 16) & 0xffff;
	}
	public static short HighHalf(int value) {
		return (short)HighHalfAsInt(value);
	}
	public static int asInt(short highHalf, short lowHalf) {
		return (highHalf << 16) | (lowHalf & 0xffff);
	}
	
	// BytePair
	// HighByte = Left
	public static int HighByte(int value) {
		return (value >>> 8) & 0xff;
	}
	// LowByte = Right
	public static int LowByte(int value) {
		return value & 0xff;
	}
	public static short asShort(int leftByte, int rightByte) {
		return (short)((leftByte << 8) | (rightByte & 0xff));
	}
	
	// NibblePair
	public static int LeftNibble(int value) {
		return (value >>> 4) & 0xf;
	}
	public static int RightNibble(int value) {
		return value & 0xf;
	}
	public static byte asByte(int left, int right) {
		return (byte)((left << 4) | (right & 0x0f));
	}
	
	// class file enumeration
    private final static char DOT = '.';
    private final static char SLASH = '/';
    private final static String CLASS_SUFFIX = ".class";

    public final static List<Class<?>> findClass(final String packageName) {
         final ClassLoader classLoader = Thread.currentThread().getContextClassLoader();
        final String scannedPath = packageName.replace(DOT, SLASH);
        final Enumeration<URL> resources;
        try {
            resources = classLoader.getResources(scannedPath);
        } catch (IOException e) {
            throw new MesaAssertionError();
        }
        final List<Class<?>> classes = new LinkedList<Class<?>>();
        while (resources.hasMoreElements()) {
            final File file = new File(resources.nextElement().getFile());
            classes.addAll(findClassInternal(file, packageName));
        }
        return classes;
    }

    private final static List<Class<?>> findClassInternal(final File file, final String scannedPackage) {
        final List<Class<?>> classes = new LinkedList<Class<?>>();
        if (file.isDirectory()) {
            for (File nestedFile : file.listFiles()) {
             	if (nestedFile.isDirectory()) {
                    classes.addAll(findClassInternal(nestedFile, scannedPackage + DOT + nestedFile.getName()));
            	} else {
            		classes.addAll(findClassInternal(nestedFile, scannedPackage));
            	}
            }
        } else if (file.getName().endsWith(CLASS_SUFFIX)) {
	        final String resource = scannedPackage + DOT + file.getName();

	        final int beginIndex = 0;
            final int endIndex = resource.length() - CLASS_SUFFIX.length();
            final String className = resource.substring(beginIndex, endIndex);
            try {
                classes.add(Class.forName(className));
            } catch (ClassNotFoundException e) {
            	//logger.warn("e = {}", e);
            }
        }

         return classes;
    }
    
    
    public static short Shift(short data, int count) {
    	if (0 < count) return (16 <= count)  ? 0 : (short)(data << count);
    	// Java promote short to int implicitly while evaluate expression.
    	// To prevent implicit sign extension, need to mask with 0xffff for short value.
     	if (count < 0) return (count <= -16) ? 0 : (short)((data & 0xffff) >>> -count);
    	return data;
    }

     public static int LongShift(int data, int count) {
    	if (0 < count) return (32 <= count)  ? 0 : (data <<   count);
     	if (count < 0) return (count <= -32) ? 0 : (data >>> -count);
    	return data;
    }
    public static int LongArithShift(int data, int count) {
    	if (0 < count) {
    		if (32 <= count) return 0;
    		return ((data << count) & 0x7fffffff) | (data & 0x80000000);
    	}
    	if (count < 0) {
    		if (count <= -32) count = -31;
    		return (data >> (-count));
    	}
    	return data;
    }
    public static short Rotate(short data, int count) {
    	if (0 < count) {
    		if (16 <= count) count = count % 16;
        	// Java promote short to int implicitly while evaluate expression.
        	// To prevent implicit sign extension, need to mask with 0xffff for short value.
    		int t = (data & 0xffff) << count;
    		return (short)((t & (0xffff)) | ((t >>> 16) & 0xffff));
    	}
    	if (count < 0) {
    		if (count <= -16) count = -(-count % 16);
        	// Java promote short to int implicitly while evaluate expression.
        	// To prevent implicit sign extension, need to mask with 0xffff for short value.
    		int t = (data & 0xffff) << (16 + count);
    		return (short)((t & (0xffff)) | ((t >>> 16) & 0xffff));
    	}
    	return data;

    }

}
