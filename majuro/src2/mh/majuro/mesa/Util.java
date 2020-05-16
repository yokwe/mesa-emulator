package mh.majuro.mesa;

import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.util.Enumeration;
import java.util.LinkedList;
import java.util.List;

import mh.majuro.UnexpectedException;

public class Util {
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
            throw new UnexpectedException();
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
}
