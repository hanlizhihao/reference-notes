package annotations;

import java.io.IOException;
import java.util.Set;

import javax.annotation.processing.AbstractProcessor;
import javax.annotation.processing.RoundEnvironment;
import javax.annotation.processing.SupportedAnnotationTypes;
import javax.annotation.processing.SupportedSourceVersion;
import javax.lang.model.SourceVersion;
import javax.lang.model.element.TypeElement;



@SupportedAnnotationTypes("annotations.ToElement")
@SupportedSourceVersion(SourceVersion.RELEASE_8)
public class ToElementAnnotationProcessor extends AbstractProcessor {
    @Override
    public boolean process(Set<? extends TypeElement> annotations,
                           RoundEnvironment currentRound) {
        if (annotations.size() == 0) return true;
        currentRound.getElementsAnnotatedWith(ToElement.class).stream().forEach(e -> {
            if (e instanceof TypeElement) {
                TypeElement typeElement = (TypeElement) e;
                System.out.println(typeElement.getSimpleName());
            }
        });
        return true;
    }

}