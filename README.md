ragel
=====

RagelX is an extension of [*Ragel*](http://www.complang.org/ragel/). Actually
it a front-end to ragel which does not give you full power on how to control
actions. Restricting actions simplify developement task as well as debugging.

Currently it use the C Preprocessor to avoid parsing a new language, thus `gcc`
or a standard `cpp` is required, but are not part of maven dependencies.

It exists a Maven plug-in to integrate RagelX compilation in a `pom.xml` file: [`ragel`](http://github.com/morandat/ragel).

Maven Coordinates:

    <repository>
      <id>se</id>
      <url>https://se.labri.fr/maven/</url>
      <!-- For some reason maven 3.0.4 seems required -->
    </repository>

    <build>
      <plugins>
        <plugin>
          <groupId>fr.labri.ragel</groupId>
          <artifactId>ragelx</artifactId>
          <!-- A ''stable'' 0.1 version is also avialable -->
          <version>0.2-SNAPSHOT</version>
        </plugin>
      </plugins>
    </build>
